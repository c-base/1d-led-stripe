// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define LED_DATA_PIN   D2
#define BUTTON_1_PIN   D6
#define BUTTON_2_PIN   D7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      48

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  while (!Serial); // wait for serial port to connect. Needed for native USB port only  
}

void clearLeds() {
  const int range = 6;
  
  for(int i = 0; i < NUMPIXELS; ++i) {
    int red = 0;
    int green = 0;
    int blue = 0;

    int b1 = digitalRead(BUTTON_1_PIN);
    int b2 = digitalRead(BUTTON_2_PIN);

    // Player 1
    if(i >= (NUMPIXELS - range)) {
      red   = b1 ? 16 : 0;
      green = 0;
      blue  = b1 ? 0 : 16;
    }
    else if(i < range) {
      red   = b2 ? 16: 0;
      green = 0;
      blue  = b2 ? 0 :16;
    }
    
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
}

enum direction {
  UP,
  DOWN
};

void loop() {
  const int ledCount = NUMPIXELS;
  static int ledPos = 0;
  static int direction = UP;
  
  for(int i = 0; i < ledCount; i++){
    clearLeds();
    pixels.setPixelColor(ledPos, pixels.Color(0,150,0)); // Moderately bright green color.        
  }

  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds).

  if(direction == UP)
    ledPos++;
  else
    ledPos--;

  if(ledPos == 0)
    direction = UP;
  
  if(ledPos == NUMPIXELS - 1)
    direction = DOWN;
}

