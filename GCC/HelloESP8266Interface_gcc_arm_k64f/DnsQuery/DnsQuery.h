/* 
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
#ifndef __DNSQUERY_H__
#define __DNSQUERY_H__

#include "SocketInterface.h"
class DnsQuery
{
public:
    /** Constructor to instantiate a DnsQuery object.
      * @param wifi : A valid pointer to a UDP socket
      */
    DnsQuery(SocketInterface* sock,const char* hostname, char* ipaddress);
 
   
private:   
    /** Function gethostbyname implements the functionality to query a domain name server for an IP-Address of a given hostname.
      * @param hostname : the hostname of interest as a string - format must be without http:// or www. IE google.com, mbed.org, etc
      * @param ipaddress : a reference to a IPADDRESS_t object which will receive the resolved IP Address of the host in question.
      * @returns true if successful, or false otherwise.
      */
    bool getHostByName(const char* hostname, char* ipaddress);
    /** Function gethostbyname implements the functionality to query a domain name server for an IP-Address of a given hostname.
      * @param hostname : the hostname of interest as a string.
      * @param ipaddress : a reference to a IPADDRESS_t object which will receive the resolved IP Address of the host in question.
      * @returns true if successful, or false otherwise.
      */
    bool getIP(const char* hostname, char* ipaddress);
    bool resolve(char* resp, char* ipaddress);
    bool parseRR(char *resp, int& c, char* adr );

protected:
    char* _dnsip;
    char* _string_ip;
    SocketInterface* socket;
    
};

#endif // __DNSQUERY_H__