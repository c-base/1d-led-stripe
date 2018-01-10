#include "1dpong.h"

//-----------------------------------------------------
// Ball
//-----------------------------------------------------

void Ball::hit(int speed) {
  // TODO: change ball direction
}

void Ball::tick() {
  // TODO: move the ball, based on the speed
}

int Ball::getPos() {
  return pos_;
}

//-----------------------------------------------------
// OneDimensionalPong
//-----------------------------------------------------

OneDimensionalPong::OneDimensionalPong() : pixels_(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800) {
  
}

void OneDimensionalPong::init() {
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  pixels_.begin();
  turnOffAllLeds();
}

void OneDimensionalPong::checkButtons() {
  int b1 = digitalRead(BUTTON_1_PIN);
  int b2 = digitalRead(BUTTON_2_PIN);

  if (ledPos_ >= (NUM_LEDS - RANGE)) { // Player 1
    if (b1) {
      direction_ = Direction::Down;

      int ledsInBase = NUM_LEDS - ledPos_;
      int i = 1 + 10 * (NUM_LEDS - ledsInBase);
      int speed = maxDelayVal - i;
    }
  }
  else if (ledPos_ < RANGE) { // Player 2
    if (b2) {
      direction_ = Direction::Up;

      int ledsInBase = ledPos_;
      int i = 1 + 10 * (ledsInBase);
      int speed = maxDelayVal - i;
    }
  }
}

void OneDimensionalPong::turnOffAllLeds() {
  pixels_.clear();
  pixels_.show();
}

void OneDimensionalPong::die() {
  turnOffAllLeds();

  // Blink
  for (int i = 0; i < 3; i++) {
    pixels_.setPixelColor(ledPos_, pixels_.Color(4, 0, 0));
    pixels_.show();
    delay(500);
    turnOffAllLeds();
    delay(500);
  }

  ledPos_ = NUM_LEDS / 2;
  direction_ = direction_ == Direction::Up ? Direction::Down : Direction::Up;
}

void OneDimensionalPong::clearLeds() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    int red = 0;
    int green = 0;
    int blue = 0;

    // Player 1
    if (i >= (NUM_LEDS - RANGE) || (i < RANGE)) {
      red   = 0;
      green = 0;
      blue  = 2;
    }

    pixels_.setPixelColor(i, pixels_.Color(red, green, blue));
  }
}

void OneDimensionalPong::tick() {
  for (int i = 0; i < ledCount_; i++) {
    clearLeds();
    pixels_.setPixelColor(ledPos_, pixels_.Color(0, 4, 0)); // Moderately bright green color.
  }

  pixels_.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds).

  if (direction_ == Direction::Up)
    ledPos_++;
  else
    ledPos_--;

  if (ledPos_ == 0)
    die();

  if (ledPos_ == NUM_LEDS - 1)
    die();

  checkButtons();
}

