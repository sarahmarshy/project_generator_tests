/* Copyright (c) 2013 Henry Leinen (henry[dot]leinen [at] online [dot] de)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "mbed.h"
#include "DnsQuery.h"

//Debug is disabled by default
#if 0
#define DBG(x, ...)  printf("[DNS : DBG]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#define WARN(x, ...) printf("[DNS : WARN]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#define ERR(x, ...)  printf("[DNS : ERR]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#else
#define DBG(x, ...) //wait_us(10);
#define WARN(x, ...) //wait_us(10);
#define ERR(x, ...)
#endif

#if 0
#define INFO(x, ...) printf("[DNS : INFO]"x" \t[%s,%d]\r\n", ##__VA_ARGS__,__FILE__,__LINE__); 
#else
#define INFO(x, ...)
#endif



DnsQuery::DnsQuery(SocketInterface *sock,const char* hostname, char* ipaddress)
{
    socket = sock;
    this->getHostByName(hostname, ipaddress);
}

bool DnsQuery::getHostByName(const char* hostname, char* resolvedIp)
{
    char * dnsIPs[] = {
        "8.8.8.8",
        "209.244.0.3",
        "84.200.69.80",
        "8.26.56.26",
        "208.67.222.222"
    };
    for(int i = 0; i<5; i++){
        _dnsip = dnsIPs[i];
        if(this->getIP(hostname, resolvedIp)){
            return true;   
        }
           
    }
    return false;
    
}

bool DnsQuery::getIP(const char* hostname, char* ipaddress)
{
    INFO("%s", hostname);
    int len = 0;
    if (hostname == NULL)
        return false;
    len = strlen(hostname);
    if ((len >128) || (len == 0))
        return false;
    
    int packetlen = /* size of HEADER structure */ 12 + /* size of QUESTION Structure */5 + len + 1;
    char *packet = new char[packetlen]; /* this is the UDP packet to send to the DNS */
    if (packet == NULL)
        return false;
    
    //  Fill the header
    memset(packet, 0, packetlen);
    packet[1] = 1;      //  ID = 1
    packet[5] = 1;      // QDCOUNT = 1 (contains one question)
    packet[2] = 1;      // recursion requested

    int c = 13;         //  point to NAME element in question section or request
    int cnt = 12;       //  points to the counter of
    packet[cnt] = 0;
    for (int i = 0 ; i < len ; i++) {
        if (hostname[i] != '.') {
            //  Copy the character and increment the character counter
            packet[cnt]++;
            packet[c++] = hostname[i];
        } else {
            //  Finished with this part, so go to the next
            cnt = c++;
            packet[cnt] = 0;
        }
    }
    
    //  Terminate this domain name with a zero entry
    packet[c++] = 0;
    
    //  Set QTYPE
    packet[c++] = 0;
    packet[c++] = 1;
    //  Set QCLASS
    packet[c++] = 0;
    packet[c++] = 1;
    
    
    //  Ready to send to DNS
    socket->setAddressPort(_dnsip, 53);
    socket->open();
   
    INFO("Sending packet of length %d",packetlen);
    /*
    for( int i = 0 ; i < c ; i++) {
        printf("%02x ", packet[i]);
    }
    */
    if (socket->send(packet, packetlen) < 0) {
        delete packet;
        return false;
    }
    delete packet;
    
    packet = new char [1024];
    
    //  Receive the answer from DNS
    int response_length = 0;
    INFO("Recieving");
    response_length = socket->recv(packet, 1024);
        /*
        for( int i = 0 ; i < 1024; i++) {
            printf("%02x ", packet[i]);
        }
        */
    if (response_length > 0 ) {
        if (!resolve(packet, ipaddress)) {
            socket->close();
            delete packet;
            ERR("NO IP FOUND\n");
            return false;
        }
                        
        //  cleanup and return
        delete packet;
        socket->close();
        return true;
    } else {
        ERR("SocketRecvFrom returned %d !", response_length);
    }
    socket->close();
    delete packet;
    ERR("NO IP FOUND\n");
    return false;
}


bool DnsQuery::resolve(char* resp, char* ipaddress)
{
    
    int ID = (((int)resp[0]) <<8) + resp[1];
    int QR = resp[2] >>7;
    int Opcode = (resp[2]>>3) & 0x0F;
    int RCODE = (resp[3] & 0x0F);
    int ANCOUNT = (((int)resp[6])<<8)+ resp[7];
    
    INFO("Resolving response : ID = %d, QR = %d, Opcode = %d, RCODE = %d", ID, QR, Opcode, RCODE);
    if ((ID != 1) || (QR != 1) || (Opcode != 0) || (RCODE != 0)) {
        ERR("Received non matching response from DNS !");
        return false;
    }
    
    int c = 12;
    int d;
    //  Skip domain question
    while(  (d=resp[c++]) != 0) {
        c+=d;
    }
    c+= 4; //   skip QTYPE and QCLASS
    
    //  Here comes the resource record
    for (int ans = 0 ; ans < ANCOUNT; ans++) {
        if (parseRR(resp, c, ipaddress)) {
            return true;
        }
    }
    
    return false;
}

bool DnsQuery::parseRR(char *resp, int& c, char* adr )
{
    int n = 0;
    while( (n=resp[c++]) != 0) {
        if ((n & 0xc0) != 0) {
            //  This is a link
            c++;
            break;
        } else {
            c+= n;  //  skip this segment, not interested in string domain names
        }
    }
    
    int TYPE = (((int)resp[c])<<8) + resp[c+1];
    int CLASS = (((int)resp[c+2])<<8) + resp[c+3];
    int RDLENGTH = (((int)resp[c+8])<<8) + resp[c+9];

    INFO("Record of TYPE=%d and CLASS=%d detected !", TYPE, CLASS);
    c+= 10;
    if ((CLASS == 1) && (TYPE == 1)) {
        sprintf(adr,"%d.%d.%d.%d", resp[c], resp[c+1], resp[c+2], resp[c+3]);
        c+= RDLENGTH;
        return true;
    } else {
    }
    c+= RDLENGTH;
    
    return false;
}