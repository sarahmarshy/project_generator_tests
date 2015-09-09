/* SocketInterface Base Class
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

#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H

#include "stdint.h"

/** This enum defines the possible socket domain types.
 */
typedef enum {
    SOCK_IPV4,      /*!< IPv4 */
    SOCK_IPV6,      /*!< IPV6 */
} socket_domain_t;

/** This enum defines the possible socket types.
 */
typedef enum {
    SOCK_STREAM,    /*!< Reliable stream-oriented service or Stream Sockets */
    SOCK_DGRAM,     /**< Datagram service or Datagram Sockets */
    SOCK_SEQPACKET, /*!< Reliable sequenced packet service */
    SOCK_RAW        /*!< Raw protocols atop the network layer */
} socket_type_t;

/** This enum defines the ip protocols
 */
typedef enum {
    SOCK_TCP,    /*!< Socket connection over TCP */
    SOCK_UDP,    /*!< Socket connection over UDP */
} socket_protocol_t;

/** Base class that defines an endpoint (TCP/UDP/Server/Client Socket)
 */
class Endpoint
{
public:

    /** Get the ip address of a host by DNS lookup
        @param name The name of a host you need an ip address for
        @return The ip address of the host otherwise NULL
     */
    virtual const char *getHostByName(const char *name) const = 0;

    /** Set the address of this endpoint
        @param addr The endpoint address
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual void setAddress(const char* addr) = 0;

    /** Set the port of this endpoint
        @param port The endpoint port
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual void setPort(uint16_t port) = 0;

    /** Set the address and port of this endpoint
        @param addr The endpoint address (supplied as an ip address).
        @param port The endpoint port
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual void setAddressPort(const char* addr, uint16_t port) = 0;

    /** Get the IP address of this endpoint
        @return The IP address of this endpoint.
     */
    virtual const char *getAddress(void) const = 0;

    /** Get the port of this endpoint
        @return The port of this socket
     */
    virtual uint16_t getPort(void) const = 0;
protected:
    const char* _addr;
    uint16_t _port;
};

/** SocketInterface class.
 *   This is a common interface that is shared between all sockets that connect
 *   using the NetworkInterface.
 */
class SocketInterface : public Endpoint
{
public:
    /** In server mode, set which port to listen on
        @param port The endpoint port
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual int32_t bind(uint16_t port) const = 0;

    /** In server mode, start listening to a port
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual int32_t listen(void) const = 0;

    /** In server mode, accept an incoming connection
        @param endpoint The endpoint we are listening to
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual int32_t accept() const = 0;

    /** In client mode, open a connection to a remote host
        @param endpoint The endpoint we want to connect to
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual int32_t open() = 0;

    /** In client or server mode send data
        @param data A buffer of data to send
        @param amount The amount of data to send
        @param timeout_ms The longest amount of time this send can take
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual int32_t send(const void *data, uint32_t amount, uint32_t timeout_ms = 15000) = 0;

    /** In client or server mode receive data
        @param data a buffer to store the data in
        @param amount The amount of data to receive
        @param timeout_ms The longest time to wait for the data
        @return The amount of data received
     */
    virtual uint32_t recv(void *data, uint32_t amount, uint32_t timeout_ms = 15000) = 0;

    /** In client or server mode, close an open connection
        @param endpoint The endpoint we want to connect to
        @return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    virtual int32_t close() const = 0;

    /** Get the socket's unique handle
        @return socket's handle number
    */
    virtual uint32_t getHandle() const = 0;

protected:
    socket_protocol_t _type;
    uint32_t _handle;
};

#endif
