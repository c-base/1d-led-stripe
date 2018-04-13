// #include "c-flo.h"
// #include "c-flo-workaround.h"
// #include "ota.h"
#include "1dpong.h"

OneDimensionalPong _pongGame;
// Cflow _cflo("mainhall", "espong", "espong");

const char* mqtt_server = "c-beam.cbrp3.c-base.org";

void onGameStart() {
  // _cflo.sendGameStartMsg();
}

void onBallHit(int pos, String direction, int speed) {
  // _cflo.sendBallHitMsg(pos, direction, speed);
}

void onPlayerVictory(int playerId) {
  // _cflo.sendPlayerVictoryMsg(playerId);
}

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for native USB port only

  // setupOta();
  // _cflo.init();

  OneDimensionalPong::PongCallbacks cb{0};
  cb.onGameStart     = onGameStart;
  cb.onBallHit             = onBallHit;
  cb.onPlayerVictory = onPlayerVictory;
  
  _pongGame.setCallbacks(cb);  
  _pongGame.init();  
}

void loop() {
  // ArduinoOTA.handle();  
  _pongGame.tick();
  // _cflo.tick();
}

