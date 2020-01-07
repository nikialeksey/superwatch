#include "matrix.h"

void Matrix::clear() {
  for (int i = 0; i < rows * columns; i++) {
    this->leds[i] = CRGB::Black;
  }
}

void Matrix::drawPixel(int x, int y, Color color) {
  // 0, 0 -> 0
  // 1, 0 -> 1
  // ...
  // 7, 0 -> 7
  // 0, 1 -> 15
  // 1, 1 -> 14
  // 2, 1 -> 13
  // ...
  // 7, 1 -> 8

  if (y % 2 == 0) {
    this->leds[y * this->rows + x] = CRGB(color.cR(), color.cG(), color.cB());
  } else {
    this->leds[y * this->rows + (this->columns - 1 - x)] = CRGB(color.cR(), color.cG(), color.cB());
  }
}
