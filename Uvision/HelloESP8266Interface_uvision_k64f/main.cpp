/* NetworkSocketAPI Example Program
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "ESP8266Interface.h"

DigitalOut myled(LED1);
void flash(){myled = !myled;}

ESP8266Interface wifi(D1, D0);

int main()
{
    Ticker t;
    //t.attach(flash, 0.2f);
    printf("NetworkSocketAPI Example\r\n");

    wifi.init();
    wifi.connect("iotlab", "42F67YxLX4AawRdcj");
    
    char* ip;
    ip = wifi.getIPAddress();
    printf("IP Address is: %s\n", (ip) ? ip : "No IP");
    char host_ip[50];
    wifi.getHostByName("google.com",host_ip);
    printf("Google.com resolved to: %s\n",host_ip);
    
    /*
    SocketInterface* mySocket = wifi.allocateSocket(SOCK_TCP);
    char recieved[100];
    int recv_amnt = 0;
    
    //Sending and receiving from echo server 
    mySocket->setAddressPort("192.168.13.8", 7);
    mySocket->open();
    mySocket->send("Hello",5, 10000);
    recieved[5] = 0;
    recv_amnt = mySocket->recv(recieved, 5, 10000);
    printf("Recieved: %d, %s\n",recv_amnt, recieved);
    mySocket->close();
    */
    wifi.disconnect();
}
