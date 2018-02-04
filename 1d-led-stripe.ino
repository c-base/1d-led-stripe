#include <PubSubClient.h>
#include <Msgflo.h>
#include "ota.h"
#include "1dpong.h"

OneDimensionalPong _pongGame;

// Mqtt
WiFiClient wifiClient;
PubSubClient mqttClient;
msgflo::Engine* engine;
msgflo::OutPort* buttonPort;
msgflo::InPort* ledPort;

const char* mqttHost = "c-beam.cbrp3.c-base.org";
const int mqttPort = 1883;
const String prefix = "mainhall/espong";
const String role = "button/one";

auto participant = msgflo::Participant("mainhall/espong", role);

void setupMsgFlo() {    
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
}

void msgFloTick() {
  static int lastTickTime = millis();

  if(millis() - lastTickTime > 1000) {
    buttonPort->send("true");
    lastTickTime = millis();
  }
  
  engine->loop();
}

// -

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for native USB port only

  setupMsgFlo();
  setupOta();
  _pongGame.init();

  buttonPort->send("true");
}

void loop() {
  ArduinoOTA.handle();  
  _pongGame.tick();
  msgFloTick();
}

