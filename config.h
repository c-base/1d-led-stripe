// ---------------------------------------------------------------
// !WARNING! >> DO NOT CHECK IN LOG IN DATA INTO GIT! << !WARNING!
// ---------------------------------------------------------------

#ifndef _CONFIG_H
#define _CONFIG_H

// WIFI:
static const char* wifi_ssid = <put ssid here>
static const char* wifi_password = <put wifi password here>
static const char* wifi_ota_name = <put OTA name here>
static const char* wifi_ota_password = <put OTA password here>

// LED stripe:
#define LED_DATA_PIN D2 // Data pin where LED stripe is connected
#define NUM_LEDS 49     // Number of LEDs on the stripe
#define BUTTON_1_PIN D7 // Button of player 1
#define BUTTON_2_PIN D6 // Button of Player 2
#define RANGE 6         // Number of base LEDs per player

#endif //_CONFIG_H

