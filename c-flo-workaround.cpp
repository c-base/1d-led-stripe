#include <ArduinoJson.h>
#include "config.h"
#include "c-flo-workaround.h"

Cflow::Cflow(String location, String name, String role) : mqttClient_(wifiClient_) {
  prefix_ = location + "/" + name + "/";
  role_ = role;    
}

void Cflow::init() {
  mqttClient_.setServer(mqttHost, 1883);  
  mqttClient_.connect((role_ + "/online").c_str(), "c-base/espong/online", 1, true, "false");

  sendOnlineMsg();
}

void Cflow::tick() {   
  static int lastTickTime = 0;

  // Send discovery message for every 5 seconds:
  // CHECKME: waiting for longer does not work !?
  
  if((lastTickTime == 0) || (millis() - lastTickTime > 5000)) {
    lastTickTime = millis();
    sendDiscoveryMsg();
  }
}

void Cflow::sendOnlineMsg() {
  const char* pPayload = "true";
  int payloadLen = strlen(pPayload);
  
  mqttClient_.publish("c-base/espong/online", reinterpret_cast<const uint8_t*>(pPayload), payloadLen, true);
}

void Cflow::sendGameStartMsg() {
  uint8_t* p = nullptr;
  mqttClient_.publish("c-base/espong/OnGameStart", p, 0);
}

void Cflow::sendBallHitMsg(int pos, String direction, int speed) {
  StaticJsonBuffer<512> rootBuffer;
  JsonObject& root = rootBuffer.createObject();
  root["position"]  = pos;
  root["direction"] = direction.c_str();
  root["speed"] = speed;

  char pMqtt[512];
  root.printTo(pMqtt); 
  mqttClient_.publish("c-base/espong/OnBallHit", pMqtt, root.measureLength());  
}

void Cflow::sendPlayerVictoryMsg(int playerId) {
  StaticJsonBuffer<512> rootBuffer;
  JsonObject& root = rootBuffer.createObject();
  root["playerId"]  = playerId;
  
  char pMqtt[512];
  root.printTo(pMqtt);
  mqttClient_.publish("c-base/espong/OnPlayerVictory", pMqtt, root.measureLength());
}

void Cflow::sendDiscoveryMsg() {  
  StaticJsonBuffer<1024> rootBuffer;
  JsonObject& root = rootBuffer.createObject();
  root["protocol"]  = "discovery";
  root["command"]   = "participant";

  JsonObject& payload  = root.createNestedObject("payload");
  payload["component"] = "c-base/espong";
  payload["label"]     = "1D pong game";
  payload["icon"]      = "gamepad";

  JsonArray& inports    = payload.createNestedArray("inports");
  JsonArray& outports   = payload.createNestedArray("outports");

  JsonObject& outport   = outports.createNestedObject();  
  outport["queue"]       = "mainhall/espong";
  outport["type"]        = "boolean";
  outport["description"] = "Shows if the node mcu is up and running";
  outport["id"]          = "online";

  JsonObject& outport2   = outports.createNestedObject();  
  outport2["queue"]       = "mainhall/espong";
  outport2["type"]        = "null";
  outport2["description"] = "Is send when a new game starts";
  outport2["id"]          = "OnGameStart";

  JsonObject& outport3   = outports.createNestedObject();
  outport3["queue"]       = "mainhall/espong";
  outport3["type"]        = "object";
  outport3["description"] = "Is send when the ball is hit";
  outport3["id"]          = "OnBallHit";

  JsonObject& outport4   = outports.createNestedObject();
  outport4["queue"]       = "mainhall/espong";
  outport4["type"]        = "object";
  outport4["description"] = "Is send when a game is over";
  outport4["id"]          = "OnPlayerVictory";
  
  payload["role"] = role_;
  payload["id"]   = role_;

  char pMqtt[1024];
  root.printTo(pMqtt); 
  mqttClient_.publish("fbp", pMqtt, root.measureLength());
}

