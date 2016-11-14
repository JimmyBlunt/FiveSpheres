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


#include "Config.h"
#include "Color.h"

#include <Adafruit_NeoPixel.h>


/// A single color scene
///
class Scene {

public:
  /// Create a new empty (black) scene.
  ///
  Scene() {
  }

  /// Copy constructor
  ///
  Scene(const Scene &other) {
    for (int i = 0; i < cNumberOfPixels; ++i) {
      _colors[i] = other._colors[i];
    }
  }

  /// Assign a new scene
  ///
  void operator=(const Scene &other) {
    for (int i = 0; i < cNumberOfPixels; ++i) {
      _colors[i] = other._colors[i];
    }
  }

  /// Set a single color in the scene.
  ///
  void setColor(int index, Color color) {
    if (index >= 0 && index < cNumberOfPixels) {
      _colors[index] = color;
    }
  }

  /// Write the scene to the pixels.
  ///
  void writeToPixels(Adafruit_NeoPixel &pixels) {
    for (int i = 0; i < cNumberOfPixels; ++i) {
      pixels.setPixelColor(i, _colors[i].getColor());
    }
    pixels.show();
  }

  /// Write the scene to the pixels, blended with another scene.
  ///
  void writeToPixels(Adafruit_NeoPixel &pixels, const Scene &nextScene, float blendFactor) {
    for (int i = 0; i < cNumberOfPixels; ++i) {
      pixels.setPixelColor(i, _colors[i].blendWith(nextScene._colors[i], blendFactor).getColor());
    }
    pixels.show();
  }

  /// Write the scene to the pixels, blended with another scene.
  ///
  void writeToPixelsFlashBlend(Adafruit_NeoPixel &pixels, const Scene &nextScene, float blendFactor) {
    const float shiftWidth = 0.3f;
    const float numberOfSpheres = static_cast<float>(cNumberOfPixels/2);
    Color whiteColor(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < cNumberOfPixels; ++i) {
      const float s = static_cast<float>(i/2)/numberOfSpheres
        + static_cast<float>(i/2)*shiftWidth;
      float f = (blendFactor/shiftWidth)-s;
      if (f < 0.0f) {
        f = 0.0f;
      }
      if (f > 1.0f) {
        f = 1.0f;
      }
      if (f < 0.5f) {
        pixels.setPixelColor(i, _colors[i].blendWith(whiteColor, f*2.0f).getColor());        
      } else {
        pixels.setPixelColor(i, whiteColor.blendWith(nextScene._colors[i], (f-0.5f)*2.0f).getColor());        
      }
    }
    pixels.show();
  }

private:
  /// Bring the value always into the range 0.0-1.0.
  ///
  static float normalize(float value) {
      while (value >= 1.0f) {
        value -= 1.0f;
      }
      while (value < 0.0f) {
        value += 1.0f;
      }
      return value;
  }


public:
  /// Scene with one single color.
  ///
  static Scene singleColor(const Color &color) {
    Scene scene;
    for (int i = 0; i < cNumberOfPixels; ++i) {
      scene.setColor(i, color);
    }
    return scene;
  }

  /// Scene with base color, other colors with additional steps.
  ///
  static Scene baseWithShift(float baseColorHue, float hueShift) {
    Scene scene;
    for (int i = 0; i < cNumberOfPixels; ++i) {
      const float shiftFactor = static_cast<float>(i/2);
      const float colorHue = normalize(baseColorHue + (hueShift * shiftFactor));
      scene.setColor(i, Color(colorHue));
    }
    return scene;
  }

  /// Scene with base color, other colors with additional steps.
  ///
  static Scene baseWithShift(float baseColorHue, float hueShift1, float hueShift2) {
    Scene scene;
    const float shifts[5] = {hueShift1, hueShift2, hueShift2, hueShift2, hueShift1};
    float hue = normalize(baseColorHue);
    for (int i = 0; i < cNumberOfPixels; ++i) {
      scene.setColor(i, Color(hue));
      if ((i & 1) != 0) {
        hue = normalize(hue + shifts[i/2]);
      }
    }
    return scene;
  }

  /// Zebra
  ///
  static Scene zebra(float colorHue, float hueShift) {
    Scene scene;
    const Color c1 = Color(colorHue);
    const Color c2 = Color(normalize(colorHue+hueShift));
    for (int i = 0; i < cNumberOfPixels; ++i) {
      if ((i&1)==0) {
        scene.setColor(i, c1);
      } else {
        scene.setColor(i, c2);
      }
    }
    return scene;
  }

  /// Two color pattern
  ///
  static Scene twoColors(float colorHue, float hueShift, uint8_t bitMask) {
    Scene scene;
    const Color c1 = Color(colorHue);
    const Color c2 = Color(normalize(colorHue+hueShift));
    for (int i = 0; i < cNumberOfPixels; ++i) {
      if (((1<<(i/2))&bitMask)==0) {
        scene.setColor(i, c1);
      } else {
        scene.setColor(i, c2);
      }
    }
    return scene;
  }

  /// Scene with white
  ///
  static Scene white(float level, float shift) {
    Scene scene;
    for (int i = 0; i < cNumberOfPixels; ++i) {
      scene.setColor(i, Color(level, level, level));
      if ((i&1)!=0) {
        level = normalize(level + shift);
      }
    }
    return scene;
  }

private:
  Color _colors[cNumberOfPixels];
};


