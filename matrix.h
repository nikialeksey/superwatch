#pragma once
#include "color.h"
#include <FastLED.h>

class Matrix {
  public:
    Matrix(int rows, int columns, CRGB* leds) {
      this->rows = rows;
      this->columns = columns;
      this->leds = leds;
    }
    void clear();
    void drawPixel(int x, int y, Color color);
  private:
    int rows;
    int columns;
    CRGB* leds;
};
