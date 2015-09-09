/* WiFiInterface Base Class
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

#ifndef WIFIINTERFACE_H
#define WIFIINTERFACE_H

#include "NetworkInterface.h"

/* wifi_security_t enum for encryption types
 */
typedef enum wifi_security_t {
    WI_NONE = 0,    /*!< No security for connection */
    WI_WEP,         /*!< WEP  encryption */
    WI_WPA,         /*!< WPA  encryption */
    WI_WPA2,        /*!< WPA2 encryption */
} wifi_security_t;

/** WiFiInterface class.
    This is a common interface to handle how WiFi connects to an access point
 */
class WiFiInterface : public NetworkInterface
{
public:

    // make sure to import the base symbol that needs an implementation for classes that have ap and phrase in the constructor
    using NetworkInterface::connect;
        
    /** Start the interface using ap name, phrase and security attributes
        @param ap Name of the network the radio should connect to
        @param pass_pharase The security phrase needed for connecting to the network
        @param security Type of encryption the network requires for connection
        @param timeout_ms Time in miliseconds to wait while attempting to connect before timing out
        @returns 0 on success, a negative number on failure
     */
    virtual int32_t connect(const char *ap, const char *pass_phrase = 0, wifi_security_t security = WI_NONE, uint32_t timeout_ms = 15000)  = 0;
    
};

#endif
