/* NetworkInterface Base Class
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

#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include "stdint.h"
#include "SocketInterface.h"
#include <map>

/** NetworkInterface class.
    This is a common interface that is shared between all hardware that connect
    to a network over IP.
 */
class NetworkInterface
{
public:

    /** Initialize the network interface with DHCP.
        @returns 0 on success, a negative number on failure
     */
    virtual int32_t init(void) = 0;

    /** Initialize the network interface with a static IP address.
        @param ip The static IP address to use
        @param mask The IP address mask
        @param gateway The gateway to use
        @returns 0 on success, a negative number on failure
     */
    virtual int32_t init(const char *ip, const char *mask, const char *gateway) = 0;
    

    /** Start the interface, using DHCP if needed.
        @param timeout_ms Time in miliseconds to wait while attempting to connect before timing out
        @returns 0 on success, a negative number on failure
     */
    virtual int32_t connect(uint32_t timeout_ms = 15000) = 0;

    /** Stop the interface, bringing down dhcp if necessary.
        @returns 0 on success, a negative number on failure
     */
    virtual int32_t disconnect(void) = 0;

    /** Get the current IP address.
        @returns a pointer to a string containing the IP address.
     */
    virtual char *getIPAddress(void) = 0;

    /** Get the current gateway address.
        @returns a pointer to a string containing the gateway address.
     */
    virtual char *getGateway(void) const = 0;


    /** Get the current network mask.
        @returns a pointer to a string containing the network mask.
     */
    virtual char *getNetworkMask(void) const = 0;

    /** Get the devices MAC address.
        @returns a pointer to a string containing the mac address.
     */
    virtual char *getMACAddress(void) const = 0;

    /** Get the current status of the interface connection.
        @returns true if connected, a negative number on failure
     */
    virtual int32_t isConnected(void) = 0;
    
    /** Allocate space for a socket.
        @param  The type of socket you want to open, SOCK_TCP or SOCK_UDP
        @returns a pointer to the allocated socket.
     */
    virtual SocketInterface* allocateSocket(socket_protocol_t socketProtocol) = 0;
    
    /** Deallocate space for a socket.
        @param  An allocated SocketInterface
        @returns 0 if object is destroyed, -1 otherwise
     */
    virtual int deallocateSocket(SocketInterface* socket) = 0;
    
protected:
    /** Map used to keep track of all SocketInterface instances */
    std::map<uint32_t, SocketInterface*> sockets;
    
    /** Counter used to create unique handles for new sockets.
        Should be incremented whenever a new socket is created. 
     */
    uint32_t uuidCounter;
};

#endif
