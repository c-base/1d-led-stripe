#ifndef __CONFIG_H
#define __CONFIG_H

// ---------------------------------------------------------------
// !WARNING! >> DO NOT CHECK IN LOG IN DATA INTO GIT! << !WARNING!
// ---------------------------------------------------------------

// WIFI:
static const char* wifi_ssid = <put ssid here>
static const char* wifi_password = <put wifi password here>
static const char* wifi_ota_name = <put OTA name here>
static const char* wifi_ota_password = <put OTA password here>

// c-flo
static const char* mqttHost = "c-beam.cbrp3.c-base.org";
static const int mqttPort = 1883;

#define LED_DATA_PIN D2 // Data pin where LED stripe is connected on Node MCU

// LED Stripe:

// Common settings:
// ================
// Bar
// -----------------------
#define NUM_LEDS 59       // Number of LEDs on the stripe
#define BUTTON_1_PIN D6   // Button of player 1
#define BUTTON_2_PIN D7   // Button of Player 2
#define RANGE 6           // Number of base LEDs per player
#define LED_BRIGHTNESS 4  // Brightness of LEDs (255 is 100%)
#define INITIAL_SPEED 10  // Initial speed
// -------------------------

// c-lab
// -------------------------
// #define NUM_LEDS 49
// #define BUTTON_1_PIN D7
// #define BUTTON_2_PIN D6
// #define RANGE 6
// #define LED_BRIGHTNESS 16
// #define INITIAL_SPEED 10
// -------------------------

// c-lab 144 stripe
// -------------------------
// #define NUM_LEDS 144
// #define BUTTON_1_PIN D6
// #define BUTTON_2_PIN D7
// #define RANGE 6
// #define LED_BRIGHTNESS 16
// #define INITIAL_SPEED 50
// -------------------------

#endif // !__CONFIG_H

