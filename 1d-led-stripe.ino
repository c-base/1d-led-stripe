#include "ota.h"
#include "1dpong.h"

OneDimensionalPong _pongGame;

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for native USB port only
  
  setupOta();
  _pongGame.init();
}

void loop() {
  ArduinoOTA.handle();  
  _pongGame.tick();
}

