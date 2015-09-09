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

#ifndef ESP8266INTERFACE_H
#define ESP8266INTERFACE_H

#include "WiFiInterface.h"
#include "ESP8266.h"
#include "DnsQuery.h"

/** ESP8266Socket class.
    This is a ESP8266 implementation of a socket that implements the SocketInterface class.
    This mock ESP8266 hardware uses AT commands, so an ATParser is used to communicate with the hardware over serial.
    In a non-AT command set, ATParser could be replaced by RawSerial or another hardware communication interface.
 */
 
class ESP8266Socket : public SocketInterface
{
public:
    ESP8266Socket(uint32_t handle, ESP8266 &driver, socket_protocol_t type, uint8_t id);
    virtual const char *getHostByName(const char *name) const;
    virtual void setAddress(const char* addr) ;
    virtual void setPort(uint16_t port) ;
    virtual void setAddressPort(const char* addr, uint16_t port);
    virtual const char *getAddress(void) const;
    virtual uint16_t getPort(void) const;
    virtual int32_t bind(uint16_t port) const;
    virtual int32_t listen(void) const;
    virtual int32_t accept() const;
    virtual int32_t open() ;
    virtual int32_t send(const void *data, uint32_t amount, uint32_t timeout_ms = 15000) ;
    virtual uint32_t recv(void *data, uint32_t amount, uint32_t timeout_ms = 15000) ;
    virtual int32_t close() const;
    virtual uint32_t getHandle() const;
    
    uint8_t getID();
    void handleRecieve();
    
protected:
    uint8_t _id;    
    ESP8266* _driver;
};

/** ESP8266Interface class.
    This is an interface to a ESP8266 radio.
 */
class ESP8266Interface : public WiFiInterface
{
public:
    ESP8266Interface(PinName tx, PinName rx);
    virtual int32_t init(void);
    virtual int32_t init(const char *ip, const char *mask, const char *gateway);
    virtual int32_t connect(uint32_t timeout_ms) ;
    virtual int32_t connect(const char *ap, const char *pass_phrase = 0, wifi_security_t security = WI_NONE, uint32_t timeout_ms = 15000);
    virtual int32_t disconnect(void) ;
    virtual char *getIPAddress(void) ;
    virtual char *getGateway(void) const;
    virtual char *getNetworkMask(void) const;
    virtual char *getMACAddress(void) const;
    virtual int32_t isConnected(void) ;
    virtual SocketInterface *allocateSocket(socket_protocol_t socketProtocol) ;
    virtual int deallocateSocket(SocketInterface *socket) ;
    void getHostByName(const char *name, char* hostIP);
    
private:
    ESP8266 esp8266;
    static const int numSockets = 5;
    int availableID[numSockets];
    char ip[100];
};

#endif
