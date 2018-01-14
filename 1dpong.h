#ifndef __1DPONG_H
#define __1DPONG_H

#include "config.h"
#include <Adafruit_NeoPixel.h>

//-----------------------------------------------------
// Ball
//-----------------------------------------------------

class Ball {
public:
  Ball(int leftBound, int rightBound);

  void setBounds(int leftBound, int rightBound);
  void setPos(int pos);
  void hit(int speed);
  void tick();  
  int getPos() const;

private:
  int pos_{NUM_LEDS / 2};
  int speed_{250}; // TODO: pixels per second
  int leftBound_{0};
  int rightBound_{0};
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

  bool ballIsInBase(const Ball& ball);
  void ballIsInOff();
  int basePos() const;
  int numBaseLeds() const;
  bool ballIsOnLastPixel(const Ball& ball);
  
private:
  const int basePos_;
  const BaseStartingPoint baseStartingPoint_;
  static const int numBaseLeds_{6};
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

  static const int ledCount_{NUM_LEDS};
  static const int maxDelayVal = 100;
  int delayval = maxDelayVal; // pixel speed
  int ledPos_{NUM_LEDS / 2};
  Direction direction_{Direction::Up};
  Adafruit_NeoPixel pixels_;  
  Player player1_;
  Player player2_;
  Ball ball_;
};

#endif // !__1DPONG_H

