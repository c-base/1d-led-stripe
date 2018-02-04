#include "config.h"
#include "c-flo.h"

Cflo::Cflo(String location, String name, String role) {  
  prefix_ = location + "/" + name + "/";
  role_ = role;  
}

void Cflo::init() {  
  msgflo::Participant participant = msgflo::Participant(prefix_, role_);
  participant.icon = "lightbulb-o";
  
  mqttClient_.setServer(mqttHost, mqttPort);
  mqttClient_.setClient(wifiClient_);  

  String clientId = role_;
  clientId += WiFi.macAddress();
  
  pEngine_ = msgflo::pubsub::createPubSubClientEngine(participant, &mqttClient_, clientId.c_str(), NULL, NULL);    
  pButtonPort_ = pEngine_->addOutPort("button", "any", prefix_ + role_ + "/event"); 
}

void Cflo::tick() {   
  static int lastTickTime = millis();  

  if(millis() - lastTickTime > 1000) {
    pButtonPort_->send("true");
    lastTickTime = millis();
  }
  
  pEngine_->loop();
}

