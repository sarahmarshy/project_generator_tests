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

#include "ESP8266Interface.h"

ESP8266Interface::ESP8266Interface(PinName tx, PinName rx) : esp8266(tx, rx)
{
    uuidCounter = 0;
    std::fill_n(availableID, sizeof(availableID)/sizeof(int), -1);
}

int32_t ESP8266Interface::init(void)
{
    if (!esp8266.startup()) {
        return -1;
    }
    if (!esp8266.reset()) {
        return -1;
    }
    if (!esp8266.wifiMode(3)) {
        return -1;
    }
    if (!esp8266.multipleConnections(true)) {
        return -1;
    }
    return 0;
}

int32_t ESP8266Interface::init(const char *ip, const char *mask, const char *gateway)
{
    return -1;
}

int32_t ESP8266Interface::connect(uint32_t timeout_ms)
{
    return -1;
}

int32_t ESP8266Interface::connect(const char *ap, const char *pass_phrase, wifi_security_t security, uint32_t timeout_ms)
{
    esp8266.setTimeout(timeout_ms);
    if (!esp8266.dhcp(1, true)) {
        return -1;
    }
    if (!esp8266.connect(ap, pass_phrase)) {
        return -1;
    }
    return 0;
}

int32_t ESP8266Interface::disconnect(void)
{
    if (!esp8266.disconnect()) {
        return -1;
    }
    for(int i=0; i<numSockets; i++) {
        SocketInterface *socket = sockets[availableID[i]];
        deallocateSocket(socket);
    }
    return 0;
}

char *ESP8266Interface::getIPAddress(void)
{
    if(!esp8266.getIPAddress(ip)) {
        return NULL;
    }
    return ip;
}

char *ESP8266Interface::getGateway(void) const
{
    return 0;
}

char *ESP8266Interface::getNetworkMask(void) const
{
    return 0;
}

char *ESP8266Interface::getMACAddress(void) const
{
    return 0;
}

int32_t ESP8266Interface::isConnected(void)
{
    return (getIPAddress() == NULL) ? -1 : 0;
}

SocketInterface *ESP8266Interface::allocateSocket(socket_protocol_t socketProtocol)
{
    int id = -1;
    //Look through the array of available sockets for an unused ID
    for(int i=0; i<numSockets; i++) {
        if (availableID[i] == -1) {
            id = i;
            availableID[i] = uuidCounter;
            break;
        }
    }
    if (id == -1) {
        return NULL;//tried to allocate more than the maximum 5 sockets
    }
    ESP8266Socket *socket = new ESP8266Socket(uuidCounter++, esp8266, socketProtocol, (uint8_t)id);
    sockets[socket->getHandle()] = socket;
    return socket;
}

int ESP8266Interface::deallocateSocket(SocketInterface *socket)
{
    int id = (int)static_cast<ESP8266Socket*>(socket)->getID();
    availableID[id] = -1;

    std::map<uint32_t, SocketInterface*>::iterator it;

    // Check if socket is owned by WiFiRadioInterface
    it = sockets.find(socket->getHandle());

    if (it != sockets.end()) {
        // If so, erase it from the internal socket map and deallocate the socket
        sockets.erase(it);
        delete socket;
    } else {
        // Socket is not owned by WiFiRadioInterface, so return -1 error
        return -1;
    }
    return 0;
}
void ESP8266Interface::getHostByName(const char *name, char* hostIP)
{
    SocketInterface* sock = this->allocateSocket(SOCK_UDP);
    DnsQuery dns(sock, name, hostIP);
    this->deallocateSocket(sock);
}   

ESP8266Socket::ESP8266Socket(uint32_t handle, ESP8266 &driver, socket_protocol_t type, uint8_t id)
{
    _handle = handle;
    _driver = &driver;
    _type = type;
    _id = id;
}

const char *ESP8266Socket::getHostByName(const char *name) const
{
    return 0;
}

void ESP8266Socket::setAddress(const char* addr)
{
    _addr = addr;
}

void ESP8266Socket::setPort(uint16_t port)
{
    _port = port;
}

void ESP8266Socket::setAddressPort(const char* addr, uint16_t port)
{
    _addr = addr;
    _port = port;
}

const char *ESP8266Socket::getAddress(void) const
{
    return _addr;
}

uint16_t ESP8266Socket::getPort(void) const
{
    return _port;
}


int32_t ESP8266Socket::bind(uint16_t port) const
{
    return -1;
}

int32_t ESP8266Socket::listen(void) const
{
    return -1;
}

int32_t ESP8266Socket::accept() const
{
    return -1;
}

int32_t ESP8266Socket::open()
{

    string sock_type = (SOCK_UDP == _type) ? "UDP" : "TCP";
    if (!_driver->openSocket(sock_type, _id, _port, _addr)) {
        return -1;
    }
    return 0;

}

int32_t ESP8266Socket::send(const void *data, uint32_t amount, uint32_t timeout_ms)
{
    _driver->setTimeout((int)timeout_ms);
    if(!_driver->sendData(_id, data, amount)) {
        return -1;
    }
    return 0;
}

uint32_t ESP8266Socket::recv(void *data, uint32_t amount, uint32_t timeout_ms)
{
    _driver->setTimeout((int)timeout_ms);
    return _driver->recv(data, amount);
}

int32_t ESP8266Socket::close() const
{
    if (!_driver->close(_id)) {
        return -1;//closing socket not succesful
    }
    return 0;
}

uint32_t ESP8266Socket::getHandle()const
{
    return _handle;
}

uint8_t ESP8266Socket::getID()
{
    return _id;
}
