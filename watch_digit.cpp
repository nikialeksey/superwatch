#include "watch_digit.h"

void WatchDigit::updateDigit(int digit) {
  this->digit = digit;
}

void WatchDigit::updateColor(Color color) {
  this->color = color;
}

void WatchDigit::draw(Matrix matrix) {
  if (digit == 0) {
    matrix.drawPixel(x, y, color);
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x, y + 1, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x, y + 2, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x, y + 3, color);
    matrix.drawPixel(x + 1, y + 3, color);
    matrix.drawPixel(x, y + 4, color);
    matrix.drawPixel(x + 1, y + 4, color);
  } else if (digit == 1) {
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x + 1, y + 3, color);
    matrix.drawPixel(x + 1, y + 4, color);
    matrix.drawPixel(x, y + 3, color);
  } else if (digit == 2) {
    matrix.drawPixel(x, y, color);
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x, y + 1, color);
    matrix.drawPixel(x, y + 2, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x + 1, y + 3, color);
    matrix.drawPixel(x + 1, y + 4, color);
    matrix.drawPixel(x, y + 4, color);
  } else if (digit == 3) {
    matrix.drawPixel(x, y, color);
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x, y + 2, color);
    matrix.drawPixel(x + 1, y + 3, color);
    matrix.drawPixel(x + 1, y + 4, color);
    matrix.drawPixel(x, y + 4, color);
  } else if (digit == 4) {
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x + 1, y + 3, color);
    matrix.drawPixel(x, y + 2, color);
    matrix.drawPixel(x, y + 3, color);
    matrix.drawPixel(x, y + 4, color);
  } else if (digit == 5) {
    matrix.drawPixel(x, y, color);
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x, y + 2, color);
    matrix.drawPixel(x, y + 3, color);
    matrix.drawPixel(x, y + 4, color);
    matrix.drawPixel(x + 1, y + 4, color);
  } else if (digit == 6) {
    matrix.drawPixel(x, y, color);
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x, y + 1, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x, y + 2, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x, y + 3, color);
    matrix.drawPixel(x, y + 4, color);
    matrix.drawPixel(x + 1, y + 4, color);
  } else if (digit == 7) {
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x + 1, y + 3, color);
    matrix.drawPixel(x + 1, y + 4, color);
    matrix.drawPixel(x, y + 4, color);
  } else if (digit == 8) {
    matrix.drawPixel(x, y, color);
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x, y + 1, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x, y + 3, color);
    matrix.drawPixel(x + 1, y + 3, color);
    matrix.drawPixel(x, y + 4, color);
    matrix.drawPixel(x + 1, y + 4, color);
  } else if (digit == 9) {
    matrix.drawPixel(x, y, color);
    matrix.drawPixel(x + 1, y, color);
    matrix.drawPixel(x + 1, y + 1, color);
    matrix.drawPixel(x, y + 2, color);
    matrix.drawPixel(x + 1, y + 2, color);
    matrix.drawPixel(x, y + 3, color);
    matrix.drawPixel(x + 1, y + 3, color);
    matrix.drawPixel(x, y + 4, color);
    matrix.drawPixel(x + 1, y + 4, color);
  }
}
