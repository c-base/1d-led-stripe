#include "1dpong.h"

#define USE_NEW_PLAYER

//-----------------------------------------------------
// Ball
//-----------------------------------------------------

Ball::Ball() {
  setPos(NUM_LEDS / 2);
}

void Ball::setPos(int pos) {
  pos_ = pos;
  speed_ = 100;
}

void Ball::hit(int speed) {
  Serial.println("Ball::hit");
  
  speed_ = speed;
}

void Ball::tick() {
  delay(1000 / (1 + abs(speed_)));

  int newPos = pos_ + (speed_ > 0 ? 1 : -1);

  if (newPos < 0 || newPos == NUM_LEDS)
    return;

  pos_ = newPos;
}

int Ball::getPos() {
  return pos_;
}

//-----------------------------------------------------
// Player
//-----------------------------------------------------

Player::Player(int basePos) : basePos_(basePos) {
  
}

void Player::ballIsInBase(const Ball& ball) {
  
}

void Player::ballIsInOff() {
  
}

int Player::basePos() {
  return basePos_;
}

int Player::numBaseLeds() {
  return numBaseLeds_;
}

//-----------------------------------------------------
// OneDimensionalPong
//-----------------------------------------------------

OneDimensionalPong::OneDimensionalPong() : pixels_(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800),
    player1_(10), player2_(30) {
  
}

void OneDimensionalPong::init() {
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  pixels_.begin();
  turnOffAllLeds();
}

void OneDimensionalPong::checkButtons() {
  Serial.println("OneDimensionalPong::checkButtons");
  
  int b1 = digitalRead(BUTTON_1_PIN);
  int b2 = digitalRead(BUTTON_2_PIN);

  if (ball_.getPos() >= (NUM_LEDS - RANGE)) { // Player 1
    if (b1)
      ball_.hit(-25);    
  }
  else if (ball_.getPos() < RANGE) { // Player 2
    if (b2)
      ball_.hit(25);
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

void OneDimensionalPong::tick() {      
  ball_.tick();  
   
  checkButtons();

  render();
}

void OneDimensionalPong::render() {
  pixels_.clear();  
  
  // players
  for(int i = player1_.basePos(); i < player1_.basePos() + player1_.numBaseLeds(); i++)
    pixels_.setPixelColor(i, pixels_.Color(0, 0, 4)); // Moderately bright blue color.

  for(int i = player2_.basePos(); i < player2_.basePos() + player2_.numBaseLeds(); i++)
    pixels_.setPixelColor(i, pixels_.Color(0, 0, 4)); // Moderately bright blue color.

  // ball
  pixels_.setPixelColor(ball_.getPos(), pixels_.Color(0, 4, 0)); // Moderately bright green color.
   
  pixels_.show(); // This sends the updated pixel color to the hardware.
}

