#include "1dpong.h"

#define USE_NEW_PLAYER

//-----------------------------------------------------
// Ball
//-----------------------------------------------------

Ball::Ball(int leftBound, int rightBound) {
  setPos(NUM_LEDS / 2);
  setBounds(leftBound, rightBound);  
}

void Ball::setPos(int pos) {
  pos_ = pos;
  speed_ = 10;
}

void Ball::hit(int speed) {
  Serial.println("Ball::hit");
  
  speed_ = speed;
}

void Ball::tick() {
  delay(5000 / (1 + abs(speed_)));

  int newPos = pos_ + (speed_ > 0 ? 1 : -1);

  if (newPos < leftBound_ || newPos == rightBound_)
    return;

  pos_ = newPos;
}

int Ball::getPos() const {
  return pos_;
}

void Ball::setBounds(int leftBound, int rightBound) {
  leftBound_ = leftBound;
  rightBound_ = rightBound;
}

bool Ball::isMovingToRight() const {
  return speed_ > 0;
}

bool Ball::isMovingToLeft() const {
  return speed_ < 0;
}

//-----------------------------------------------------
// Player
//-----------------------------------------------------

Player::Player(int basePos, BaseStartingPoint baseStartingPoint) : basePos_(basePos), baseStartingPoint_(baseStartingPoint) {
  
}

bool Player::ballIsInBase(const Ball& ball) {
  if(baseStartingPoint_ == BaseStartingPoint::Left) {
    if(ball.getPos() < basePos_ + numBaseLeds_)
      return true;
  }
  else if(baseStartingPoint_ == BaseStartingPoint::Right) {
    if(ball.getPos() > basePos_)
      return true;
  }

  return false;
}

int Player::ballPositionInBase(const Ball& ball) const {
  if(baseStartingPoint_ == BaseStartingPoint::Left) {
    if(ball.getPos() < basePos_ + numBaseLeds_)
      return basePos_ + numBaseLeds_ - ball.getPos();      
  }
  else if(baseStartingPoint_ == BaseStartingPoint::Right) {
    if(ball.getPos() >= basePos_)
      return 1 + ball.getPos() - basePos_;
  }

  return 0;
}

void Player::ballIsInOff() {
  
}

int Player::basePos() const {
  return basePos_;
}

int Player::numBaseLeds() const {
  return numBaseLeds_;
}

bool Player::ballIsOnLastPixel(const Ball& ball) const {
  if(ball.getPos() == basePos_)
    return true;

  return false;
}

//-----------------------------------------------------
// OneDimensionalPong
//-----------------------------------------------------

OneDimensionalPong::OneDimensionalPong() : pixels_(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800),
    player1_(10, BaseStartingPoint::Left), player2_(30, BaseStartingPoint::Right), 
    ball_(player1_.basePos(), player2_.basePos() + player2_.numBaseLeds()) {
  
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

  if (player1_.ballIsInBase(ball_)) {
    if (b1 && ball_.isMovingToLeft()) {
      ball_.hit(10);
    }
  }
  else if (player2_.ballIsInBase(ball_)) {
    if (b2 && ball_.isMovingToRight()) {
      ball_.hit(-10);
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

void OneDimensionalPong::tick() {  
  Serial.print("Ball position in base Player1: ");
  Serial.println(player1_.ballPositionInBase(ball_));
  Serial.print("Ball position in base Player2: ");
  Serial.println(player2_.ballPositionInBase(ball_));
  
  checkButtons();
  ball_.tick();
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

