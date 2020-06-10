

/**
   See README.md for details

   Modify Config.h for your Wifi and Hue setup
*/
#include "Config.h"
#include <Arduino.h>
#include <EButton.h>
#include "HueClient.h"
#include "HueGroup.h"
#include "Led.h"
#include "OTA.h"
#include "qdec.h"
#include <WiFi.h>


#define USE_SERIAL Serial

int lastActivityTime = 0;
const int sleepInterval = 1200000;
bool powerToggleReady = false;
bool sceneChangeReady = false;
int sceneIndex = 0;
int rotaryCount = 0;
bool rotaryWait = false;
bool clockwiseTrue = false;
Led Onboard(13);
EButton powerButton(POWER_PIN);
EButton sceneButton(SCENE_PIN);
::SimpleHacks::QDecoder hddRotary(ROTARY_PIN_A, ROTARY_PIN_B, true);
HueClient conn(HUE_IP, HUE_API_USERNAME);
HueGroup group(conn, HUE_GROUP);


void powerHandler(EButton &btn) {
  powerToggleReady = true;
}

void sceneHandler(EButton &btn) {
  sceneIndex = (btn.getClicks() - 1);
  sceneChangeReady = true;
}

void setup()
{
  USE_SERIAL.begin(115200);
  // Connect to wifi
  Onboard.toggle();
  ArduinoOTA.setHostname("klewHueSketch");
  setupOTA("klewHueSketch");
  powerButton.attachSingleClick(powerHandler);
  sceneButton.attachDoneClicking(sceneHandler);
  hddRotary.begin();
  Onboard.toggle();
}

void loop() {
  ArduinoOTA.handle();
  powerButton.tick();
  sceneButton.tick();
  ::SimpleHacks::QDECODER_EVENT event = hddRotary.update();

  if (powerToggleReady) {
    group.toggle();
    powerToggleReady = false;
    lastActivityTime = millis();
    Onboard.on();
    while (millis() < (lastActivityTime + 100)){}
    Onboard.off();
  }

  if (sceneChangeReady) {
    group.setScene(scenes[sceneIndex]);
    sceneChangeReady = false;
    lastActivityTime = millis();
    Onboard.on();
    while (millis() < (lastActivityTime + 100)){}
    Onboard.off();
  }

  if (event & ::SimpleHacks::QDECODER_EVENT_CW) {
    rotaryCount = (clockwiseTrue) ? rotaryCount + 1 : 0;
    clockwiseTrue = true;
  } else if (event & ::SimpleHacks::QDECODER_EVENT_CCW) {
    rotaryCount = (!clockwiseTrue) ? rotaryCount + 1 : 0;
    clockwiseTrue = false;
  }

  if (rotaryCount >= 10) {
    Onboard.on();
    group.scrollBrightnessStart();
    if (clockwiseTrue) {
      group.increaseBrightness();
    }
    else {
      group.decreaseBrightness();
    }
    rotaryCount = 0;
    Onboard.off();
  }
}
