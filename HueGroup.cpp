#include "Arduino.h"
#include "HueGroup.h"

#define USE_SERIAL Serial

HueGroup::HueGroup(HueClient& c, int group)
  : _conn(c)
  , _group(group)
  , _brightnessAscending(false)
{
}

void HueGroup::toggle()
{
  char endpoint[30];

  sprintf(endpoint, "/groups/%d", _group);
  String response = _conn.request("GET", endpoint);

  sprintf(endpoint, "/groups/%d/action", _group);
  if (response.indexOf("\"any_on\":false") > -1) {
    on();
  } else {
    off();
  }
}

void HueGroup::on()
{
  char endpoint[30];
  sprintf(endpoint, "/groups/%d/action", _group);
  _conn.request("PUT", endpoint, "{\"on\": true}");
}

void HueGroup::off()
{
  char endpoint[30];
  sprintf(endpoint, "/groups/%d/action", _group);
  _conn.request("PUT", endpoint, "{\"on\": false}");
}

void HueGroup::scrollBrightnessStart()
{
    char endpoint[30];
    sprintf(endpoint, "/groups/%d", _group);
    String response = _conn.request("GET", endpoint);

    String part = response.substring(response.indexOf("\"bri\"") + 6);
    _brightness = part.substring(0, part.indexOf(",")).toInt();
    if (response.indexOf("\"any_on\":false") > -1) {
        _brightness = part.substring(0, part.indexOf(",")).toInt();
    }
}

void HueGroup::increaseBrightness()
{
  char endpoint[30];
  sprintf(endpoint, "/groups/%d/action", _group);

  if (_brightness < 254) {
    _conn.request("PUT", endpoint, "{\"bri_inc\": 5}");
    _brightness += 5;
    on();
  }
  else {
    _brightness = 254;
    on();
  }
}

void HueGroup::decreaseBrightness()
{
  char endpoint[30];
  sprintf(endpoint, "/groups/%d/action", _group);
  if (_brightness > 0) {
    _conn.request("PUT", endpoint, "{\"bri_inc\": -5}");
    _brightness += -5;
    on();
    
  }
  else {
    _brightness = 5;
    off();
  }
}

void HueGroup::setScene(char* scene)
{
  char endpoint[30];
  sprintf(endpoint, "/groups/%d/action", _group);

  char payload[80];
  sprintf(payload, "{\"scene\": \"%s\"}", scene);

  _conn.request("PUT", endpoint, payload);
}
