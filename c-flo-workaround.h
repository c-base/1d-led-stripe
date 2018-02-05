#ifndef _CFLO_WORKAROUND_H
#define _CFLO_WORKAROUND_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Workaround: The output buffer is only 128 byte and therefore too small for c-flo auto discovery messages.
//             It has to be increased in the PubSubClient.h to at least 512 bytes until it gets fixed by the developers:

#if MQTT_MAX_PACKET_SIZE < 512
  #error "Incrase MQTT_MAX_PACKET_SIZE in PubSubClient.h before compiling"
#endif

class Cflow {
public:
  Cflow(String location, String name, String role);
  void init();
  void tick();

private:
  void sendDiscoveryMsg();
  void sendOnlineMsg();

  WiFiClient wifiClient_;
  PubSubClient mqttClient_;
  String prefix_;
  String role_;
};

#endif // !_CFLO_WORKAROUND_H

