#pragma once
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


#include <Arduino.h>


/// The pin where the NeoPixels are connected.
///
const uint8_t cLedPin = 12;

/// The number of pixels attached to the strand.
///
/// Other parts of the code rely on 10 pixels. If you change this
/// make sure everything else is working with the different number
/// of pixels.
///
const uint8_t cNumberOfPixels = 10;

/// The number of seconds to keep one color scene.
///
/// The default value is 60 for 60 secod
///
const uint8_t cKeepSceneInSeconds = 60;

/// The speed to change from one scene to another.
///
/// The blend time is: blendTime = 1.0/cBlendSpeed * 20ms
/// The default value is 0.002, which is a 10 second blend.
///
const float cBlendSpeed = 0.002f;

/// The pin where the light sensor is connected.
///
/// It is expected the light sensor is an analog sensor producing
/// a voltage between 0 and 3.3V on the analog input.
///
const uint8_t cLightSensorPin = 4;

/// The value from which the environment brightness is too high 
/// for the decoration to switch to day mode.
///
/// The value 0xffff disables the day mode. If you have a light
/// sensor attached, use e.g. a value like 80 to enable the day
/// mode, but you need to experiment with your sensor to find the
/// right value.
///
const uint16_t cLightThreshold = 0xffff; //=off

/// The number of cycles to wait before switching to the daylight mode.
///
/// This guarantees a stable switch between day and night mode, even 
/// if there are smaller bightness changes.
///
const uint8_t cLightSwitchDelay = 3;



