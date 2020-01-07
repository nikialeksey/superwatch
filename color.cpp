#include "color.h"

int Color::cR() {
  return this->r;
}

int Color::cG() {
  return this->g;
}

int Color::cB() {
  return this->b;
}

void Color::clear() {
  this->r = 0;
  this->g = 0;
  this->b = 0;
}
