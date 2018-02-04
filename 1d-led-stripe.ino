#include <PubSubClient.h>
#include "c-flo.h"
#include "ota.h"
#include "1dpong.h"

OneDimensionalPong _pongGame;
Cflo _cflo("mainhall", "espong", "button/one");

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for native USB port only

  setupOta();
  _pongGame.init();  
  _cflo.init();
}

void loop() {
  ArduinoOTA.handle();  
  _pongGame.tick();
  _cflo.tick();
}

