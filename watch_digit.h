#pragma once
#include "color.h"
#include "matrix.h"

class WatchDigit {
  public:
    WatchDigit(int x, int y, Color color, int digit): color(color) {
      this->x = x;
      this->y = y;
      this->digit = digit;
    }
    void updateDigit(int digit);
    void updateColor(Color color);
    void draw(Matrix matrix);
  private:
    int x;
    int y;
    Color color;
    int digit;
};
