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
  static int lastTickTime = millis();

  // Send discovery message for every 30 seconds:
  
  if(millis() - lastTickTime > 30 * 1000) {    
    lastTickTime = millis();
    sendDiscoveryMsg();
  }
}

void Cflow::sendOnlineMsg() {
  const char* pPayload = "true";
  int payloadLen = strlen(pPayload);
  
  mqttClient_.publish("c-base/espong/online", reinterpret_cast<const uint8_t*>(pPayload), payloadLen, true);
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
  
  payload["role"] = role_;
  payload["id"]   = role_;

  char pMqtt[512];
  root.printTo(pMqtt); 
  mqttClient_.publish("fbp", pMqtt, root.measureLength());
}

