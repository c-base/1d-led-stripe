// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include "config.h"
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

enum direction {
  UP,
  DOWN
};

static const int ledCount = NUM_LEDS;
static int ledPos = NUM_LEDS / 2;
static int direction = UP;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // delay for half a second

void setupOta() {
  Serial.println("Booting");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(wifi_ota_name);

  // No authentication by default
  ArduinoOTA.setPassword(wifi_ota_password);

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  Serial.println("bla\n");
  
  setupOta();  
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  

  while (!Serial); // wait for serial port to connect. Needed for native USB port only  
}

void clearLeds() {    
  for(int i = 0; i < NUM_LEDS; ++i) {
    int red = 0;
    int green = 0;
    int blue = 0;     

    // Player 1
    if(i >= (NUM_LEDS - RANGE) || (i < RANGE)) {
      red   = 0;
      green = 0;
      blue  = 16;
    }
    
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
}

void turnOffAllLeds() {
  for(int i = 0; i < NUM_LEDS; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0));

  pixels.show();
}

void die() {
  turnOffAllLeds();

  // Blink
  for(int i = 0; i < 3; i++) {  
    pixels.setPixelColor(ledPos, pixels.Color(255,0,0));
    pixels.show();
    delay(500);
    turnOffAllLeds();
    delay(500);
  }

  ledPos = NUM_LEDS / 2;
  direction = direction == UP ? DOWN : UP;  
}

void checkButtons() {
  int b1 = digitalRead(BUTTON_1_PIN);
  int b2 = digitalRead(BUTTON_2_PIN);
  
  if(ledPos >= (NUM_LEDS - RANGE)) { // Player 1
    if(b1)
      direction = DOWN;  
  }
  else if(ledPos < RANGE) { // Player 2
    if(b2)
      direction = UP;  
  }
}

void loop() {    
  ArduinoOTA.handle();
  
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
    die();
  
  if(ledPos == NUM_LEDS - 1)
    die();    

  checkButtons();
}


