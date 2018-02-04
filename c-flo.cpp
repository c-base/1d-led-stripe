#include <ESP8266WiFi.h>
#include "config.h"
#include "c-flo.h"

const String prefix = "mainhall/espong";
const String role = "button/one";

auto participant = msgflo::Participant("mainhall/espong", role);

WiFiClient wifiClient;
PubSubClient mqttClient;
msgflo::Engine* engine;
msgflo::OutPort* buttonPort;
msgflo::InPort* ledPort;

void setupCflo() {    
  participant.icon = "toggle-on";

  mqttClient.setServer(mqttHost, mqttPort);
  mqttClient.setClient(wifiClient);

  String clientId = role;
  clientId += WiFi.macAddress();
  engine = msgflo::pubsub::createPubSubClientEngine(participant, &mqttClient, clientId.c_str(), NULL, NULL);
  buttonPort = engine->addOutPort("button", "any", prefix + role + "/event");

  ledPort = engine->addInPort("led", "boolean", prefix + role + "/led",
    [](byte *data, int length) -> void {
      const std::string in((char *)data, length);
      const boolean on = (in == "1" || in == "true");     
    });

  buttonPort->send("true");
}

void cFloTick() {
  static int lastTickTime = millis();

  if(millis() - lastTickTime > 1000) {
    buttonPort->send("true");
    lastTickTime = millis();
  }
  
  engine->loop();
}

