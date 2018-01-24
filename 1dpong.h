#ifndef __1DPONG_H
#define __1DPONG_H

#include "config.h"
#include <string>
#include <Adafruit_NeoPixel.h>

//-----------------------------------------------------
// Ball
//-----------------------------------------------------

typedef void (*OnBallHitBoundCallback)(void* pInstnace, int pos);

enum class Direction {
  Left,
  Right
};

class Ball {
public:
  Ball(int leftBound, int rightBound, 
      void* pCallbackInstance = nullptr, OnBallHitBoundCallback pOnBallHitBoundCallback = nullptr);

  void setBounds(int leftBound, int rightBound);
  void setPos(int pos, Direction direction = Direction::Right);
  void hit(int speed);
  void tick();
  int getPos() const;
  bool isMovingToRight() const;
  bool isMovingToLeft() const;

private:
  int pos_{NUM_LEDS / 2};
  int speed_{250}; // TODO: pixels per second
  int leftBound_{0};
  int rightBound_{0};
  Direction direction_{Direction::Right};

  void* pCallbackInstance_{nullptr};
  OnBallHitBoundCallback pOnBallHitBoundCallback_{nullptr};
};

//-----------------------------------------------------
// Player
//-----------------------------------------------------

enum class BaseStartingPoint {
  Left,
  Right
};

class Player {
public:
  Player(int basePos, BaseStartingPoint startingPoint);

  void setName(const char* pName);
  bool ballIsInBase(const Ball& ball);  
  int basePos() const;
  int numBaseLeds() const;
  int ballPositionInBase(const Ball& ball) const;
  bool ballIsOnLastPixel(const Ball& ball) const;
  const char* getName() const;
  void kill();
  void revive();
  void resetLifes();
  bool isDead() const ;
  int lifes() const;
      
private:
  static const int numBaseLeds_{RANGE};
  int numLifes_{numBaseLeds_};
  bool isDead_{false};
  const int basePos_;
  const BaseStartingPoint baseStartingPoint_;
  
  std::string name_{"anonymous"};  
};

//-----------------------------------------------------
// OneDimensionalPong
//-----------------------------------------------------

class OneDimensionalPong {
public:
  OneDimensionalPong();
  void init();  
  void checkButtons();
  void turnOffAllLeds();  
  void die();  
  void tick();
  void render();

private:  
  enum class Direction {
    Up,
    Down
  };

  static void onBallHitBounds(void* pInstance, int pos);
  
  static const int ledCount_{NUM_LEDS};
  static const int maxDelayVal = 100;
  int delayval = maxDelayVal; // pixel speed  
    
  Adafruit_NeoPixel pixels_;  
  Player player1_;
  Player player2_;
  Ball ball_;
};

#endif // !__1DPONG_H

