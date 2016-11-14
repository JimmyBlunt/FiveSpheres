//
// Five Spheres
// ---------------------------------------------------------------------------
// (c)2016 by Lucky Resistor. See LICENSE for details.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//


#include "Config.h"
#include "Color.h"
#include "Scene.h"

#include <Adafruit_NeoPixel.h>


/// The instance of the NEO pixel library.
///
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(cNumberOfPixels, cLedPin, NEO_GRB + NEO_KHZ800);


/// The current visible scene.
///
Scene currentScene;

/// The next scene.
///
Scene nextScene;

/// The current mode of the device
///
enum Mode {
  NightMode,
  DayMode
} mode = NightMode;

/// The daymode counter.
///
/// If the light exceeds the threshold for over 15 minutes, the lights turn off.
///
uint8_t dayModeCounter = 0;


/// Setup everything.
///
void setup() {
  // Setup the serial console.
  Serial.begin(115200);
  delay(1000);
  Serial.println("Welcome!");

  // Configure the inputs
  pinMode(cLightSensorPin, INPUT);
  
  // Initialize the neo pixel library
  pixels.begin();

  // Initialize the random number generator to start
  // with another number for each start. Using unused analog
  // inputs which will read more or less random values.
  randomSeed(analogRead(0)^
    (((uint32_t)analogRead(1))<<8)^
    (((uint32_t)analogRead(2))<<16));
  
  // Generate a random initial scene
  currentScene = generateRandomScene();
}


/// The main loop.
///
void loop() {
  switch (mode) {
  case NightMode: nightMode(); break;
  case DayMode: dayMode(); break;
  }
}


/// The night mode
///
void nightMode() {
  
  // Show the current scene.
  currentScene.writeToPixels(pixels);

  // Keep the current scene for a defined number of seconds.
  for (int i = 0; i < cKeepSceneInSeconds; ++i) {
    delay(1000);
  }

  // Read the light value
  if (isDaylight()) {
    ++dayModeCounter;
    if (dayModeCounter > cLightSwitchDelay) {
      Serial.println("Switch to day mode.");
      mode = DayMode;
      // Fade out.
      nextScene = Scene::singleColor(Color(0.0, 0.0, 0.0));
      blendToNextScene();    
      ++dayModeCounter = 0;
      return; // skip the rest.
    }
  } else {
    dayModeCounter = 0;
  }

  // Prepare the next scene
  nextScene = generateRandomScene();
  blendToNextScene();    
}


/// The day mode. Wait for the night.
///
void dayMode() {

  // Show the current scene.
  currentScene.writeToPixels(pixels);

  // Wait for some seconds.
  for (int i = 0; i < cKeepSceneInSeconds; ++i) {
    delay(1000);
  }
  
  // Read the light value
  if (!isDaylight()) {
    ++dayModeCounter;
    if (dayModeCounter > cLightSwitchDelay) {
      Serial.println("Switch to night mode.");
      mode = NightMode;
      // Fade a new random scene in.
      nextScene = generateRandomScene();
      blendToNextScene();    
      dayModeCounter = 0;
    }
  } else {
    dayModeCounter = 0;
  }  
}


/// Check if it is daylight
///
bool isDaylight() {
  const uint16_t lightValue = analogRead(cLightSensorPin);
  Serial.print("light value = ");
  Serial.print(lightValue);
  Serial.print(", daylight = ");
  const bool daylight = lightValue > cLightThreshold;
  Serial.println(daylight ? "yes" : "no");
  return daylight;
}


/// Blend to the next scene and replace the current scene with it.
///
void blendToNextScene() {
  for (float i = 0.0f; i < 1.0f; i += cBlendSpeed) {
    delay(20);
    currentScene.writeToPixelsFlashBlend(pixels, nextScene, i);
  }
  currentScene = nextScene;
}


/// Get a random factor value.
///
float getRandomFactor() {
  return (float)(random(1024))/1024.0f;
}


/// Generate a new random color scene.
///
Scene generateRandomScene() {
  const int variant = random(9);
  const float randomHue = getRandomFactor();
  const float randomShift1 = getRandomFactor();
  const float randomShift2 = getRandomFactor();
  switch (variant) {
  case 0:
    return Scene::singleColor(Color(randomHue)); 
  case 1:
    return Scene::baseWithShift(randomHue, randomShift1/4.0f);
  case 2:
    return Scene::baseWithShift(randomHue, -(randomShift1/4.0f));
  case 3:
    return Scene::baseWithShift(randomHue, randomShift1/5.0f, randomShift2/3.0);
  case 4:
    return Scene::baseWithShift(randomHue, -(randomShift1/5.0f), -(randomShift2/3.0));
  case 5:
    return Scene::zebra(randomHue, randomShift1);
  case 6:
    return Scene::twoColors(randomHue, randomShift1, 0b00100);
  case 7:
    return Scene::twoColors(randomHue, randomShift1, 0b10001);
  case 8:
    return Scene::twoColors(randomHue, randomShift1, 0b10101);
  }
  
  return Scene();
}



