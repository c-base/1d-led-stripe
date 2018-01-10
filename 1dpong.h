#ifndef __1DPONG_H
#define __1DPONG_H

#include "config.h"
#include <Adafruit_NeoPixel.h>

class OneDimensionalPong {
public:
  OneDimensionalPong();
  void init();  
  void checkButtons();
  void turnOffAllLeds();
  void clearLeds();
  void die();  
  void tick();

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
};

#endif // !__1DPONG_H
