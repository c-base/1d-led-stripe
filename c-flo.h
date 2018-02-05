#ifndef _CFLO_H
#define _CFLO_H

#include <ESP8266WiFi.h>
#include <Msgflo.h>

// Workaround: The output buffer is only 128 byte and therefore too small for c-flo auto discovery messages.
//             It has to be increased in the PubSubClient.h to at least 1024 bytes until it gets fixed by the developers:

#if MQTT_MAX_PACKET_SIZE < 1024
  #error "Incrase MQTT_MAX_PACKET_SIZE in PubSubClient.h before compiling"
#endif

class Cflo {
public:
  Cflo(String location, String name, String role);
  void init();
  void tick();

private:
  WiFiClient wifiClient_;
  PubSubClient mqttClient_;
  msgflo::Engine* pEngine_{NULL};  
  msgflo::OutPort* pButtonPort_{NULL};
  msgflo::InPort* pLedPort_{NULL};

  String prefix_;
  String role_;
};

#endif // !_CFLO_H

