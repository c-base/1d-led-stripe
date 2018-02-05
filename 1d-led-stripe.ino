// #include "c-flo.h"
#include "c-flo-workaround.h"
#include "ota.h"
#include "1dpong.h"

OneDimensionalPong _pongGame;
Cflow _cflo("mainhall", "espong", "espong");

const char* mqtt_server = "c-beam.cbrp3.c-base.org";

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

