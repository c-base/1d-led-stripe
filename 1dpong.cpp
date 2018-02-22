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
  speed_ = 20;  
}

void Ball::hit(int speed) {
  direction_ = direction_ == Direction::Right ? Direction::Left : Direction::Right;  
  speed_ += speed;
}

void Ball::tick() {
  delay(1000 / (1 + abs(speed_)));

  // CHECKME: does crash!?
  // static int lastTime = millis();
  // if(millis() - lastTime < (1000 / (1 + abs(speed_))))
  // return;
  // lastTime = millis();

  int newPos = pos_ + (direction_ == Direction::Right ? 1 : -1);

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

int Ball::getSpeed() const {
  return speed_;
}

void Ball::setBounds(int leftBound, int rightBound) {
  leftBound_ = leftBound;
  rightBound_ = rightBound;
}

bool Ball::isMovingToRight() const {
  return direction_ == Direction::Right;
}

bool Ball::isMovingToLeft() const {
  return direction_ == Direction::Left;
}

Direction Ball::getDirection() const {
  return direction_;
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

void Player::resetLifes() {
  numLifes_ = numBaseLeds_;
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

  if(callbacks_.onGameStart)
    callbacks_.onGameStart();
}

void OneDimensionalPong::setCallbacks(const PongCallbacks& callbacks) {
  callbacks_ = callbacks;
}

void OneDimensionalPong::checkButtons() {
  static int lastB1 = 0;
  static int lastB2 = 0;
  int b1 = digitalRead(BUTTON_1_PIN);
  int b2 = digitalRead(BUTTON_2_PIN);

  if (player1_.ballIsInBase(ball_) && !lastB1) {
    if (b1 && ball_.isMovingToLeft()) {
      ball_.hit(player1_.ballPositionInBase(ball_));

      if(callbacks_.onBallHit) {
        const char* pDirection = ball_.getDirection() == Direction::Right ? "right" : "left";
        callbacks_.onBallHit(ball_.getPos(), pDirection, ball_.getSpeed());
      }
    }
  }
  else if (player2_.ballIsInBase(ball_) && !lastB2) {
    if (b2 && ball_.isMovingToRight()) {
      ball_.hit(player2_.ballPositionInBase(ball_));

      if(callbacks_.onBallHit) {
        const char* pDirection = ball_.getDirection() == Direction::Right ? "right" : "left";        
        callbacks_.onBallHit(ball_.getPos(), pDirection, ball_.getSpeed());
      }
    }
  }

  lastB1 = b1;
  lastB2 = b2;
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
    pixels_.setPixelColor(ball_.getPos(), pixels_.Color(LED_BRIGHTNESS, 0, 0));
    pixels_.show();
    delay(500);
    turnOffAllLeds();
    delay(500);
  }
  
  ball_.setPos(NUM_LEDS / 2);

  if(player1_.isDead())
    player1_.revive();
  
  if(player2_.isDead())
    player2_.revive();

  ball_.hit(INITIAL_SPEED);
}

void OneDimensionalPong::tick() {  
  checkButtons();
  ball_.tick();

  if(player1_.isDead()) {
    die();

    if(player1_.lifes() == player1_.numBaseLeds() && player2_.lifes() == player2_.numBaseLeds()) {
      if(callbacks_.onGameStart)
        callbacks_.onGameStart();    
    }
  }
  else if(player2_.isDead()) {
    die();

    if(player1_.lifes() == player1_.numBaseLeds() && player2_.lifes() == player2_.numBaseLeds()) {
      if(callbacks_.onGameStart)
        callbacks_.onGameStart();    
    }
  }
  
  render();  
}
  
void OneDimensionalPong::render() {
  pixels_.clear();
  
  // players
  for(int i = 0; i < player1_.numBaseLeds(); i++) {
    int pos = i + player1_.basePos();

    if(pos < player1_.lifes())
      pixels_.setPixelColor(pos, pixels_.Color(0, 0, LED_BRIGHTNESS));
    else
      pixels_.setPixelColor(pos, pixels_.Color(LED_BRIGHTNESS, 0, 0));
  }

  for(int i = 0; i < player2_.numBaseLeds(); i++) {
    int pos = i + player2_.basePos();
    
    if(pos - player2_.basePos() >= player2_.numBaseLeds() - player2_.lifes())
      pixels_.setPixelColor(pos, pixels_.Color(0, 0, LED_BRIGHTNESS));
    else
      pixels_.setPixelColor(pos, pixels_.Color(LED_BRIGHTNESS, 0, 0));
  }

  // ball
  pixels_.setPixelColor(ball_.getPos(), pixels_.Color(0, LED_BRIGHTNESS, 0)); // Moderately bright green color.
   
  pixels_.show(); // This sends the updated pixel color to the hardware.
}

void OneDimensionalPong::onBallHitBounds(void* pInstance, int pos) {
  OneDimensionalPong* pThis = static_cast<OneDimensionalPong*>(pInstance);  

  if(pThis->player1_.ballIsInBase(pThis->ball_)) {
    pThis->player1_.kill();

    if(pThis->player1_.lifes() == 0) {
      if(pThis->callbacks_.onPlayerVictory)
        pThis->callbacks_.onPlayerVictory(2);
    }
  }
  else if(pThis->player2_.ballIsInBase(pThis->ball_)) {
    pThis->player2_.kill();

    if(pThis->player2_.lifes() == 0) {
      if(pThis->callbacks_.onPlayerVictory)
        pThis->callbacks_.onPlayerVictory(1);
    }
  }

  if(pThis->player1_.lifes() == 0 || pThis->player2_.lifes() == 0) {
    pThis->player1_.resetLifes();
    pThis->player2_.resetLifes();    
  }
}

