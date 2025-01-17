/*
  PubSubClient.cpp - A simple client for MQTT.
  Nick O'Leary
  http://knolleary.net
*/

#include "PubSubClient.h"
#include <Arduino.h>

#ifdef ESP32
#include <WiFiClient.h>
#endif

#ifdef USE_SECOND_HEAP
  #include <umm_malloc/umm_heap_select.h>
#endif


PubSubClient::PubSubClient() {
    this->_state = MQTT_DISCONNECTED;
    this->_client = NULL;
    this->stream = NULL;
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;

    setCallback(NULL);
}

PubSubClient::PubSubClient(Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setClient(client);
    this->stream = NULL;
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}

PubSubClient::PubSubClient(IPAddress addr, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setClient(client);
    this->stream = NULL;
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setClient(client);
    setStream(stream);
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}

PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setClient(client);
    this->stream = NULL;
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setClient(client);
    setStream(stream);
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}

PubSubClient::PubSubClient(const char* domain, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    this->stream = NULL;
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    setStream(stream);
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->keepAlive_sec = MQTT_KEEPALIVE;
    this->socketTimeout_msec = MQTT_SOCKET_TIMEOUT * 1000;
}

PubSubClient::~PubSubClient()
{
    if (buffer != nullptr) {
        free(buffer);
        buffer = nullptr;
    }
}

boolean PubSubClient::connect(const char *id) {
    return connect(id,NULL,NULL,0,0,0,0,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass) {
    return connect(id,user,pass,0,0,0,0,1);
}

boolean PubSubClient::connect(const char *id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,NULL,NULL,willTopic,willQos,willRetain,willMessage,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,user,pass,willTopic,willQos,willRetain,willMessage,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage, boolean cleanSession) {
    if (!initBuffer()) {
        return false;
    }

    if (!connected()) {
        int result = 0;

        if (_client == nullptr) {
            return false;
        }
        if (_client->connected()) {
            result = 1;
        } else {
            if (domain.length() != 0) {
#if defined(ESP32) && ESP_IDF_VERSION_MAJOR < 5 
                WiFiClient* wfc = (WiFiClient*)_client;
                result = wfc->connect(this->domain.c_str(), this->port, _client->getTimeout());
#else
                result = _client->connect(this->domain.c_str(), this->port);
#endif
            } else {
#if defined(ESP32) && ESP_IDF_VERSION_MAJOR < 5 
                WiFiClient* wfc = (WiFiClient*)_client;
                result = wfc->connect(this->ip, this->port, _client->getTimeout());
#else
                result = _client->connect(this->ip, this->port);
#endif
            }
        }
        if (result == 1) {
            nextMsgId = 1;
            // Leave room in the buffer for header and variable length field
            uint16_t length = MQTT_MAX_HEADER_SIZE;
            unsigned int j;

#if MQTT_VERSION == MQTT_VERSION_3_1
            uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
            uint8_t d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 7
#endif
            for (j = 0;j<MQTT_HEADER_VERSION_LENGTH;j++) {
                buffer[length++] = d[j];
            }

            uint8_t v;
            if (willTopic) {
                v = 0x04|(willQos<<3)|(willRetain<<5);
            } else {
                v = 0x00;
            }
            if (cleanSession) {
                v = v|0x02;
            }

            if(user != NULL) {
                v = v|0x80;

                if(pass != NULL) {
                    v = v|(0x80>>1);
                }
            }

            buffer[length++] = v;

            buffer[length++] = ((keepAlive_sec) >> 8);
            buffer[length++] = ((keepAlive_sec) & 0xFF);

            CHECK_STRING_LENGTH(length,id)
            length = writeString(id,buffer,length);
            if (willTopic) {
                CHECK_STRING_LENGTH(length,willTopic)
                length = writeString(willTopic,buffer,length);
                CHECK_STRING_LENGTH(length,willMessage)
                length = writeString(willMessage,buffer,length);
            }

            if(user != NULL) {
                CHECK_STRING_LENGTH(length,user)
                length = writeString(user,buffer,length);
                if(pass != NULL) {
                    CHECK_STRING_LENGTH(length,pass)
                    length = writeString(pass,buffer,length);
                }
            }

            write(MQTTCONNECT,buffer,length-MQTT_MAX_HEADER_SIZE);

            lastInActivity = lastOutActivity = millis();
            pingOutstanding = false; // See: https://github.com/knolleary/pubsubclient/pull/802

            while (!_client->available()) {
                delay(0);  // Prevent watchdog crashes
                unsigned long t = millis();
                if ((int32_t)(t - lastInActivity) >= socketTimeout_msec) {
                    _state = MQTT_CONNECTION_TIMEOUT;
                    _client->stop();
                    return false;
                }
            }
            uint8_t llen;
            uint16_t len = readPacket(&llen);

            if (len == 4) {
                if (buffer[3] == 0) {
                    lastInActivity = millis();
                    pingOutstanding = false;
                    _state = MQTT_CONNECTED;
                    return true;
                } else {
                    _state = buffer[3];
                }
            }
            _client->stop();
        } else {
            _state = MQTT_CONNECT_FAILED;
        }
        return false;
    }
    return true;
}

// reads a byte into result
boolean PubSubClient::readByte(uint8_t * result) {
   if (_client == nullptr) {
     return false;
   }
   uint32_t previousMillis = millis();

   while(!_client->available() && _client->connected()) {
     delay(1);  // Prevent watchdog crashes

     if((int32_t)(millis() - previousMillis) >= socketTimeout_msec){
       return false;
     }
   }
   *result = _client->read();
   return true;
}

// reads a byte into result[*index] and increments index
boolean PubSubClient::readByte(uint8_t * result, uint16_t * index){
  uint16_t current_index = *index;
  uint8_t * write_address = &(result[current_index]);
  if(readByte(write_address)){
    *index = current_index + 1;
    return true;
  }
  return false;
}

uint16_t PubSubClient::readPacket(uint8_t* lengthLength) {
    if (!initBuffer()) {
        return 0;
    }

    uint16_t len = 0;
    if(!readByte(buffer, &len)) return 0;
    bool isPublish = (buffer[0]&0xF0) == MQTTPUBLISH;
    uint32_t multiplier = 1;
    uint16_t length = 0;
    uint8_t digit = 0;
    uint16_t skip = 0;
    uint8_t start = 0;

    do {
        if (len == 5) {
            // Invalid remaining length encoding - kill the connection
            _state = MQTT_DISCONNECTED;
            _client->stop();
            return 0;
        }
        if(!readByte(&digit)) return 0;
        buffer[len++] = digit;
        length += (digit & 127) * multiplier;
        multiplier *= 128;
    } while ((digit & 128) != 0 && len < (MQTT_MAX_PACKET_SIZE -2));
    *lengthLength = len-1;

    if (isPublish) {
        // Read in topic length to calculate bytes to skip over for Stream writing
        if(!readByte(buffer, &len)) return 0;
        if(!readByte(buffer, &len)) return 0;
        skip = (buffer[*lengthLength+1]<<8)+buffer[*lengthLength+2];
        start = 2;
        if (buffer[0]&MQTTQOS1) {
            // skip message id
            skip += 2;
        }
    }

    for (uint16_t i = start;i<length;i++) {
        if(!readByte(&digit)) return 0;
        if (this->stream) {
            if (isPublish && len-*lengthLength-2>skip) {
                this->stream->write(digit);
            }
        }
        if (len < MQTT_MAX_PACKET_SIZE) {
            buffer[len] = digit;
        }
        len++;
    }

    if (!this->stream && len > MQTT_MAX_PACKET_SIZE) {
        len = 0; // This will cause the packet to be ignored.
    }

    return len;
}

bool PubSubClient::loop_read() {
    if (!initBuffer()) {
        return false;
    }

    if (_client == nullptr) {
        return false;
    }
    if (!_client->available()) {
        return false;
    }
    uint8_t llen;
    uint16_t len = readPacket(&llen);
    if (len == 0) {
        return false;
    }
    unsigned long t = millis();
    lastInActivity = t;
    uint8_t type = buffer[0]&0xF0;

    switch(type) {
        case MQTTPUBLISH: 
        {
            if (callback) {
                const bool msgId_present = (buffer[0]&0x06) == MQTTQOS1;
                const uint16_t tl_offset = llen+1;
                const uint16_t tl = (buffer[tl_offset]<<8)+buffer[tl_offset+1]; /* topic length in bytes */
                const uint16_t topic_offset = tl_offset+2;
                const uint16_t msgId_offset = topic_offset+tl;
                const uint16_t payload_offset = msgId_present ? msgId_offset+2 : msgId_offset;
                if (payload_offset >= MQTT_MAX_PACKET_SIZE) return false;
                if (len < payload_offset) return false;
                // Need to move the topic 1 byte to insert a '\0' at the end of the topic.
                memmove(buffer+topic_offset-1,buffer+topic_offset,tl); /* move topic inside buffer 1 byte to front */
                buffer[topic_offset-1+tl] = 0; /* end the topic as a 'C' string with \x00 */
                char *topic = (char*) buffer+topic_offset-1;
                uint8_t *payload;
                // msgId only present for QOS>0
                if (msgId_present) {
                    const uint16_t msgId = (buffer[msgId_offset]<<8)+buffer[msgId_offset+1];
                    payload = buffer+payload_offset;
                    callback(topic,payload,len-payload_offset);
                    if (_client->connected()) {
                        buffer[0] = MQTTPUBACK;
                        buffer[1] = 2;
                        buffer[2] = (msgId >> 8);
                        buffer[3] = (msgId & 0xFF);
                        if (_client->write(buffer,4) != 0) {
                            lastOutActivity = t;
                        }
                    }
                } else {
                    // No msgId
                    payload = buffer+payload_offset;
                    callback(topic,payload,len-payload_offset);
                }
            }
            break;
        } 
        case MQTTPINGREQ:
        {
            if (_client->connected()) {
                buffer[0] = MQTTPINGRESP;
                buffer[1] = 0;
                if (_client->write(buffer,2) != 0) {
                    lastOutActivity = t;
                }
            }
            break;
        } 
        case MQTTPINGRESP:
        {
            pingOutstanding = false;
            break;
        }
        default:
        return false;
    }
    return true;
}

boolean PubSubClient::loop() {
    delay(0);
    loop_read();
    if (connected()) {
        unsigned long t = millis();
        // Send message at 2/3 of keepalive interval
        // Wait for server-sent keep-alive till 3/2 of keepalive interval
        // Just to make sure the broker will not disconnect us
        const int32_t keepalive_66pct = pingOutstanding
            ? keepAlive_sec * 1500
            : keepAlive_sec * 666;
        if (((int32_t)(t - lastInActivity) > keepalive_66pct) || 
            ((int32_t)(t - lastOutActivity) > keepalive_66pct)) {
            if (pingOutstanding) {
                this->_state = MQTT_CONNECTION_TIMEOUT;
                _client->stop();
                return false;
            } else {
                buffer[0] = MQTTPINGREQ;
                buffer[1] = 0;
                if (_client->write(buffer,2) != 0) {
                  lastOutActivity = t;
                  lastInActivity = t;
                }
                pingOutstanding = true;
            }
        }
        return true;
    }
    return false;
}

boolean PubSubClient::publish(const char* topic, const char* payload) {
    size_t plength = (payload != nullptr) ? strlen(payload) : 0;
    return publish(topic,(const uint8_t*)payload,plength,false);
}

boolean PubSubClient::publish(const char* topic, const char* payload, boolean retained) {
    size_t plength = (payload != nullptr) ? strlen(payload) : 0;
    return publish(topic,(const uint8_t*)payload,plength,retained);
}

boolean PubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength) {
    return publish(topic, payload, plength, false);
}

boolean PubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    if (!beginPublish(topic, plength, retained)) {
        return false;
    }
    for (unsigned int i=0;i<plength;i++) {
        write(payload[i]);
    }
    endPublish();
    return true;
}

boolean PubSubClient::publish_P(const char* topic, const char* payload, boolean retained) {
    size_t plength = (payload != nullptr) ? strlen_P(payload) : 0;
    return publish_P(topic, (const uint8_t*)payload, plength, retained);
}

boolean PubSubClient::publish_P(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    if (!beginPublish(topic, plength, retained)) {
        return false;
    }
    for (unsigned int i=0;i<plength;i++) {
        write(pgm_read_byte_near(payload + i));
    }
    endPublish();
    return true;
}

boolean PubSubClient::beginPublish(const char* topic, unsigned int plength, boolean retained) {
    _bufferWritePos = 0;
    if (connected()) {
        // Send the header and variable length field
        uint16_t length = MQTT_MAX_HEADER_SIZE;
        length = writeString(topic,buffer,length);
        uint8_t header = MQTTPUBLISH;
        if (retained) {
            header |= 1;
        }
        size_t hlen = buildHeader(header, buffer, plength+length-MQTT_MAX_HEADER_SIZE);
        uint16_t rc = _client->write(buffer+(MQTT_MAX_HEADER_SIZE-hlen),length-(MQTT_MAX_HEADER_SIZE-hlen));
        if (rc > 0) {
           lastOutActivity = millis();
        }
        return (rc == (length-(MQTT_MAX_HEADER_SIZE-hlen)));
    }
    return false;
}

int PubSubClient::endPublish() {
    flushBuffer();
    return 1;
}

size_t PubSubClient::write(uint8_t data) {
    if (_client == nullptr) {
        lastOutActivity = millis();
        return 0;
    }
    size_t rc = appendBuffer(data);
    if (rc != 0) {
        lastOutActivity = millis();
    }
    return rc;
}

size_t PubSubClient::write(const uint8_t *data, size_t size) {
    if (_client == nullptr) {
        lastOutActivity = millis();
        return 0;
    }
    size_t rc = appendBuffer(data,size);
    if (rc != 0) {
        lastOutActivity = millis();
    }
    return rc;
}

size_t PubSubClient::write(const String& message) {
    return write(reinterpret_cast<const uint8_t *>(message.c_str()), message.length());
}


size_t PubSubClient::buildHeader(uint8_t header, uint8_t* buf, uint32_t length) {
    uint8_t lenBuf[4];
    uint8_t llen = 0;
    uint8_t digit;
    uint8_t pos = 0;
    uint32_t len = length;
    do {
        digit = len % 128;
        len = len / 128;
        if (len > 0) {
            digit |= 0x80;
        }
        lenBuf[pos++] = digit;
        llen++;
    } while(len>0 && pos < 4);

    buf[4-llen] = header;
    for (int i=0;i<llen;i++) {
        buf[MQTT_MAX_HEADER_SIZE-llen+i] = lenBuf[i];
    }
    return llen+1; // Full header size is variable length bit plus the 1-byte fixed header
}

boolean PubSubClient::write(uint8_t header, uint8_t* buf, uint32_t length) {
    uint16_t rc;
    uint8_t hlen = buildHeader(header, buf, length);

#ifdef MQTT_MAX_TRANSFER_SIZE
    uint8_t* writeBuf = buf+(MQTT_MAX_HEADER_SIZE-hlen);
    uint16_t bytesRemaining = length+hlen;  //Match the length type
    uint8_t bytesToWrite;
    boolean result = true;
    while((bytesRemaining > 0) && result) {
        delay(0);  // Prevent watchdog crashes
        bytesToWrite = (bytesRemaining > MQTT_MAX_TRANSFER_SIZE)?MQTT_MAX_TRANSFER_SIZE:bytesRemaining;
        rc = _client->write(writeBuf,bytesToWrite);
        result = (rc == bytesToWrite);
        bytesRemaining -= rc;
        writeBuf += rc;
        if (rc != 0) {
            lastOutActivity = millis();
        }
    }
    return result;
#else
    rc = _client->write(buf+(MQTT_MAX_HEADER_SIZE-hlen),length+hlen);
    if (rc != 0) {
        lastOutActivity = millis();
    }
    return (rc == hlen+length);
#endif
}

boolean PubSubClient::subscribe(const char* topic) {
    return subscribe(topic, 0);
}

boolean PubSubClient::subscribe(const char* topic, uint8_t qos) {
    if (qos > 1) {
        return false;
    }
    if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic)) {
        // Too long
        return false;
    }
    if (connected()) {
        // Leave room in the buffer for header and variable length field
        uint16_t length = MQTT_MAX_HEADER_SIZE;
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString((char*)topic, buffer,length);
        buffer[length++] = qos;
        return write(MQTTSUBSCRIBE|MQTTQOS1,buffer,length-MQTT_MAX_HEADER_SIZE);
    }
    return false;
}

boolean PubSubClient::unsubscribe(const char* topic) {
    if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic)) {
        // Too long
        return false;
    }
    if (connected()) {
        uint16_t length = MQTT_MAX_HEADER_SIZE;
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString(topic, buffer,length);
        return write(MQTTUNSUBSCRIBE|MQTTQOS1,buffer,length-MQTT_MAX_HEADER_SIZE);
    }
    return false;
}

void PubSubClient::disconnect() {
    if (_state == MQTT_DISCONNECTED || !initBuffer()) {
        _state = MQTT_DISCONNECTED;
        lastInActivity = lastOutActivity = millis();

        return;
    }

    buffer[0] = MQTTDISCONNECT;
    buffer[1] = 0;
    if (_client != nullptr) {
      _client->write(buffer,2);
      _client->flush();
      _client->stop();
    }
    _state = MQTT_DISCONNECTED;
    lastInActivity = lastOutActivity = millis();
}

uint16_t PubSubClient::writeString(const char* string, uint8_t* buf, uint16_t pos) {
    const char* idp = string;
    uint16_t i = 0;
    pos += 2;
    while (*idp && pos < (MQTT_MAX_PACKET_SIZE - 2)) {
        buf[pos++] = *idp++;
        i++;
    }
    buf[pos-i-2] = (i >> 8);
    buf[pos-i-1] = (i & 0xFF);
    return pos;
}

size_t PubSubClient::appendBuffer(uint8_t data) {
    if (!initBuffer()) {
        return 0;
    }

    buffer[_bufferWritePos] = data;
    ++_bufferWritePos;
    if (_bufferWritePos >= MQTT_MAX_PACKET_SIZE) {
        if (flushBuffer() == 0) return 0;
    }
    return 1;
}

size_t PubSubClient::appendBuffer(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (appendBuffer(data[i]) == 0) return i;
    }
    return size;
}

size_t PubSubClient::flushBuffer() {
    size_t rc = 0;
    if (_bufferWritePos > 0) {
        if (connected()) {
            rc = _client->write(buffer, _bufferWritePos);
            if (rc != 0) {
                lastOutActivity = millis();
            }
        }
        _bufferWritePos = 0;
    }
    return rc;
}

bool PubSubClient::initBuffer()
{
    constexpr size_t size = sizeof(uint8_t) * MQTT_MAX_PACKET_SIZE;
    if (buffer == nullptr) {
#ifdef ESP32
        buffer = (uint8_t*) heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (buffer == nullptr) {
          buffer = (uint8_t*) malloc(size);
        }
#else
        {
#ifdef USE_SECOND_HEAP
            // Try allocating on ESP8266 2nd heap
            HeapSelectIram ephemeral;
#endif
            buffer = (uint8_t*) malloc(size);
            if (buffer != nullptr) return true;
        }
#ifdef USE_SECOND_HEAP
        // Not successful, try allocating on (ESP8266) main heap
        HeapSelectDram ephemeral;
#endif
        buffer = (uint8_t*) malloc(size);
#endif
    }
    return buffer != nullptr;
}

boolean PubSubClient::connected() {
    if (_client == NULL ) {
        this->_state = MQTT_DISCONNECTED;
        return false;
    }
    if (_client->connected() == 0) {
        bool lastStateConnected = this->_state == MQTT_CONNECTED;
        this->disconnect();
        if (lastStateConnected) {
            this->_state = MQTT_CONNECTION_LOST;
        }
        return false;
    }
    return this->_state == MQTT_CONNECTED;
}

PubSubClient& PubSubClient::setServer(uint8_t * ip, uint16_t port) {
    IPAddress addr(ip[0],ip[1],ip[2],ip[3]);
    return setServer(addr,port);
}

PubSubClient& PubSubClient::setServer(IPAddress ip, uint16_t port) {
    this->ip = ip;
    this->port = port;
    this->domain = "";
    return *this;
}

PubSubClient& PubSubClient::setServer(const char * domain, uint16_t port) {
    this->domain = domain;
    this->port = port;
    return *this;
}

PubSubClient& PubSubClient::setCallback(MQTT_CALLBACK_SIGNATURE) {
    this->callback = callback;
    return *this;
}

PubSubClient& PubSubClient::setClient(Client& client){
    this->_client = &client;
    return *this;
}

PubSubClient& PubSubClient::setStream(Stream& stream){
    this->stream = &stream;
    return *this;
}

int PubSubClient::state() {
    return this->_state;
}

PubSubClient& PubSubClient::setKeepAlive(uint16_t keepAlive_sec) {
    this->keepAlive_sec = keepAlive_sec;
    return *this;
}

PubSubClient& PubSubClient::setSocketTimeout(uint16_t timeout_ms) {
    this->socketTimeout_msec = timeout_ms;
    return *this;
}