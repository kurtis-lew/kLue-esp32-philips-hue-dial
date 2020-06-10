// GPIO config
#define POWER_PIN 27
#define SCENE_PIN 33 
#define ROTARY_PIN_A 15
#define ROTARY_PIN_B 32

// Wifi Config
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Hue Config
#define HUE_IP "" // IP of your hue bridge (you might want to give it a static IP on your router), see https://www.meethue.com/api/nupnp
#define HUE_API_USERNAME "" // See https://developers.meethue.com/develop/get-started-2/
#define HUE_GROUP 1 // Numeric id of the group of lights you want to control, call hue api /api/{ username }/groups for this
char scenes[4][16] = { // 15 digit ids of the scenes you want the switch to iterate through. Call hue api /api/{ username }/scenes for these
    "", // Choose only scenes available for your chosen group
    "",
    "",
    ""
};
