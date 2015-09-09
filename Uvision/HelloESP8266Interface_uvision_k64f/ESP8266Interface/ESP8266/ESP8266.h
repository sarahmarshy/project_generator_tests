/* ESP8266Interface Example
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

#ifndef ESP8266_H
#define ESP8266_H

#include "ATParser.h"
#include <string>

/** ESP8266Interface class.
    This is an interface to a ESP8266 radio.
 */
class ESP8266 
{
public:
    ESP8266(PinName tx, PinName rx);
    
    /**
    * Test startup of ESP8266
    *
    * @return true only if ESP8266 AT system working correctly
    */
    bool startup(void);
    
    /**
    * Reset ESP8266
    *
    * @return true only if ESP8266 resets successfully 
    */
    bool reset(void);
    
    /**
    * Set WiFi mode
    *
    * @param mode mode of WiFi 1-client, 2-host, 3-both
    * @return true only if ESP8266 enables/disables DHCP successfully
    */
    bool wifiMode(int mode);
    
    /**
    * Enable/Disable multiple connections
    *
    * @param enabled multiple connections enabled when true
    * @return true only if ESP8266 enables/disables multiple connections successfully
    */
    bool multipleConnections(bool enabled);
    
    /**
    * Enable/Disable DHCP
    *
    * @param mode mode of DHCP 0-softAP, 1-station, 2-both
    * @param enabled DHCP enabled when true
    * @return true only if ESP8266 enables/disables DHCP successfully
    */
    bool dhcp(int mode, bool enabled);
    
    /**
    * Connect ESP8266 to AP
    *
    * @param ap the name of the AP
    * @param passPhrase the password of AP
    * @return true only if ESP8266 is connected successfully
    */
    bool connect(const char *ap, const char *passPhrase);
    
    /**
    * Disconnect ESP8266 from AP
    *
    * @return true only if ESP8266 is disconnected successfully
    */
    bool disconnect(void);
    
    /**
    * Get the IP address of ESP8266
    *
    * @param ip data placeholder for IP address
    * @return true only if ESP8266 is assigned an IP address
    */
    bool getIPAddress(char* ip);
    
    /**
    * Check if ESP8266 is conenected
    *
    * @return true only if the chip has an IP address
    */
    bool isConnected(void);
    
    /**
    * Open a socketed connection 
    *
    * @param sockType the type of socket to open "UDP" or "TCP"
    * @param id id to give the new socket, valid 0-4
    * @param port port to open connection with
    * @param addr the IP address of the destination 
    * @return true only if socket opened successfully
    */
    bool openSocket(string sockType, int id, int port, const char* addr);
    
    /**
    * Sends data to an open socket
    *
    * @param id id of socket to send to
    * @param data data to be sent
    * @param amount amount of data to be sent - max 1024
    * @return true only if data sent successfully
    */
    bool sendData(int id, const void *data, uint32_t amount);
    
    /**
    * Receives data from an open socket 
    *
    * @param data placeholder for returned information
    * @param amount number of bytes to be received
    * @return the number of bytes actually received
    */
    uint32_t recv(void *data, uint32_t amount);
    
    /**
    * Closes a socket
    *
    * @param id id of socket to close, valid only 0-4
    * @return true only if socket is closed successfully
    */
    bool close(int id);
    
    /**
    * Allows timeout to be changed between commands
    *
    * @param timeout_ms timeout of the connection
    */
    void setTimeout(uint32_t timeout_ms);
    
private:
    BufferedSerial serial;
    ATParser atParser;
};

#endif
