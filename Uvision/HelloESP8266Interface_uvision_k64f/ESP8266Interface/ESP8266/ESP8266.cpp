/* ESP8266 Example
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

#include "ESP8266.h"

ESP8266::ESP8266(PinName tx, PinName rx) : serial(tx, rx), atParser(serial)
{
    serial.baud(115200);
    atParser.setEcho(1);
}

bool ESP8266::startup(void)
{
    return (atParser.send("AT") && atParser.recv("OK"));
}

bool ESP8266::reset(void)
{
    return (atParser.send("AT+RST") && atParser.recv("OK\r\nready"));
}

bool ESP8266::wifiMode(int mode)
{
    //only 3 valid modes
    if(mode < 1 || mode > 3) {
        return false;
    }

    char modestr[1];
    sprintf(modestr,"%d",mode);
    string mode_command = "AT+CWMODE="+string(modestr);
    return (atParser.send(mode_command.c_str()) && atParser.recv("OK"));
}

bool ESP8266::multipleConnections(bool enabled)
{
    int on = (int)enabled;
    char enable[1];
    sprintf(enable,"%d",on);
    string mux_command = "AT+CIPMUX="+string(enable);
    return (atParser.send(mux_command.c_str()) && atParser.recv("OK"));
}

bool ESP8266::dhcp(int mode, bool enabled)
{
    //only 3 valid modes
    if(mode < 0 || mode > 2) {
        return false;
    }
    int on = (int)enabled;
    char enable[1];
    sprintf(enable,"%d",on);
    char modestr[1];
    sprintf(modestr,"%d",mode);
    string mode_command = "AT+CWDHCP="+string(modestr)+","+string(enable);
    return (atParser.send(mode_command.c_str()) && atParser.recv("OK"));
}

bool ESP8266::connect(const char *ap, const char *passPhrase)
{
    string connect_command = "AT+CWJAP=\""+(string)ap+"\",\""+(string)passPhrase+"\"";
    return (atParser.send(connect_command.c_str()) && atParser.recv("OK"));
}

bool ESP8266::disconnect(void)
{
    return (atParser.send("AT+CWQAP") && atParser.recv("OK"));
}

bool ESP8266::getIPAddress(char* ip)
{
    return (atParser.send("AT+CIPSTA?") && atParser.recv("+CIPSTA:\"%[^\"]\"", ip));
}

bool ESP8266::isConnected(void)
{
    char* ip = "";
    return getIPAddress(ip);
}

bool ESP8266::openSocket(string sockType, int id, int port, const char* addr)
{
    //IDs only 0-4
    if(id > 4) {
        return false;
    }
    char portstr[5];
    char idstr[2];
    sprintf(idstr,"%d",id);
    sprintf(portstr, "%d", port);

    string start_command = "AT+CIPSTART="+(string)idstr+",\""+sockType+"\",\""+(string)addr+"\","+(string)portstr;
    if (!(atParser.send(start_command.c_str()) && atParser.recv("OK"))) {
        return false;//opening socket not succesful
    }
    return true;
}

bool ESP8266::sendData(int id, const void *data, uint32_t amount)
{
    char idstr[2];
    sprintf(idstr,"%d",id);
    char lenstr[5];
    sprintf(lenstr,"%d",(int)amount);

    string send_command = "AT+CIPSEND="+(string)idstr+","+(string)lenstr;
    if(!atParser.send(send_command.c_str(),">")) {
        return false;
    }
    wait_ms(10);
    atParser.write((char*)data, (int)amount);
    wait_ms(10);
    return true;
}

uint32_t ESP8266::recv(void *data, uint32_t amount)
{
    int length;
    int id;
    if (!(atParser.recv("+IPD,%d,%d:", &id, &length) && atParser.read((char*)data, length) && atParser.recv("OK"))) {
        return 0;
    }
    return length;
}

bool ESP8266::close(int id)
{
    //IDs only 0-4
    if(id > 4) {
        return false;
    }
    char idstr[2];
    sprintf(idstr,"%d",id);
    string close_command = "AT+CIPCLOSE="+(string)idstr;

    return (atParser.send(close_command.c_str()) && atParser.recv("OK"));
}

void ESP8266::setTimeout(uint32_t timeout_ms)
{
    atParser.setTimeout(timeout_ms);
}