#include "watch.h"

void Watch::update(int hour, int minute) {
  this->hh.updateDigit(hour / 10);
  this->hl.updateDigit(hour % 10);
  this->mh.updateDigit(minute / 10);
  this->ml.updateDigit(minute % 10);
}

void Watch::draw(Matrix matrix) {
  this->hh.draw(matrix);
  this->hl.draw(matrix);
  this->mh.draw(matrix);
  this->ml.draw(matrix);
}
