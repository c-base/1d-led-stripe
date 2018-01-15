#include "1dpong.h"

//-----------------------------------------------------
// Ball
//-----------------------------------------------------

Ball::Ball(int leftBound, int rightBound, void* pCallbackInstance, 
    OnBallHitBoundCallback pOnBallHitBoundCallback) : pCallbackInstance_(pCallbackInstance),
    pOnBallHitBoundCallback_(pOnBallHitBoundCallback) {
      
  setPos(NUM_LEDS / 2);
  setBounds(leftBound, rightBound);  
}

void Ball::setPos(int pos) {
  pos_ = pos;
  speed_ = 10;
}

void Ball::hit(int speed) {
  speed_ = speed;
}

void Ball::tick() {
  delay(1000 / (1 + abs(speed_)));

  int newPos = pos_ + (speed_ > 0 ? 1 : -1);

  if (newPos < leftBound_ || newPos == rightBound_) {
   if(pOnBallHitBoundCallback_)
     pOnBallHitBoundCallback_(pCallbackInstance_, pos_); 
  
    return;
  }

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

void Player::setName(const char* pName) {
  name_ = pName;
}

bool Player::ballIsInBase(const Ball& ball) {
  return ballPositionInBase(ball) != 0;
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

int Player::basePos() const {
  return basePos_;
}

int Player::numBaseLeds() const {
  return numBaseLeds_;
}

bool Player::ballIsOnLastPixel(const Ball& ball) const {
  return ball.getPos() == basePos_;
}

void Player::kill() {
  Serial.print("Player '");
  Serial.print(name_.c_str());
  Serial.println("' is dying");

  numLifes_--;
  isDead_ = true;
}

bool Player::isDead() const {
  return isDead_;
}

void Player::revive() {
  isDead_ = false;
}

const char* Player::getName() const {
  return name_.c_str();
}

int Player::lifes() const {
  return numLifes_;
}

//-----------------------------------------------------
// OneDimensionalPong
//-----------------------------------------------------

OneDimensionalPong::OneDimensionalPong() : pixels_(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800),
    player1_(0, BaseStartingPoint::Left), player2_(NUM_LEDS - RANGE, BaseStartingPoint::Right), 
    ball_(player1_.basePos(), player2_.basePos() + player2_.numBaseLeds(), this, onBallHitBounds) {

  player1_.setName("Player 1");
  player2_.setName("Player 2");  
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

  if (player1_.ballIsInBase(ball_)) {
    if (b1 && ball_.isMovingToLeft())
      ball_.hit(10 * player1_.ballPositionInBase(ball_));          
  }
  else if (player2_.ballIsInBase(ball_)) {
    if (b2 && ball_.isMovingToRight())
      ball_.hit(-10 * player2_.ballPositionInBase(ball_));    
  }
}

void OneDimensionalPong::turnOffAllLeds() {
  pixels_.clear();
  pixels_.show();
  delay(50);
}

void OneDimensionalPong::die() {
  turnOffAllLeds();

  // Blink
  for (int i = 0; i < 3; i++) {
    pixels_.setPixelColor(ball_.getPos(), pixels_.Color(4, 0, 0));
    pixels_.show();
    delay(500);
    turnOffAllLeds();
    delay(500);
  }
  
  ball_.setPos(NUM_LEDS / 2);

  if(player1_.isDead()) {    
    player1_.revive();  
    ball_.hit(10);
  }

  if(player2_.isDead()) {    
    player2_.revive();  
    ball_.hit(-10);
  }  
}

void OneDimensionalPong::tick() {  
  checkButtons();
  ball_.tick();

  if(player1_.isDead()) {
    die();
  }
  else if(player2_.isDead()) {
    die();
  }
  
  render();  
}
  
void OneDimensionalPong::render() {
  pixels_.clear();
  
  // players
  for(int i = 0; i < player1_.numBaseLeds(); i++) {
    int pos = i + player1_.basePos();

    if(pos < player1_.lifes())
      pixels_.setPixelColor(pos, pixels_.Color(0, 0, 4));      
    else
      pixels_.setPixelColor(pos, pixels_.Color(4, 0, 4));
  }

  for(int i = 0; i < player2_.numBaseLeds(); i++) {
    int pos = i + player2_.basePos();
    
    if(pos - player2_.basePos() >= player2_.numBaseLeds() - player2_.lifes())
      pixels_.setPixelColor(pos, pixels_.Color(0, 0, 4));
    else
      pixels_.setPixelColor(pos, pixels_.Color(4, 0, 4));      
  }

  // ball
  pixels_.setPixelColor(ball_.getPos(), pixels_.Color(0, 4, 0)); // Moderately bright green color.
   
  pixels_.show(); // This sends the updated pixel color to the hardware.
}

void OneDimensionalPong::onBallHitBounds(void* pInstance, int pos) {
  OneDimensionalPong* pThis = static_cast<OneDimensionalPong*>(pInstance);  

  if(pThis->player1_.ballIsInBase(pThis->ball_))
    pThis->player1_.kill();
  else if(pThis->player2_.ballIsInBase(pThis->ball_))
    pThis->player2_.kill();  
}

