#pragma once
#include "watch_digit.h"
#include "matrix.h"
#include "color.h"

class Watch {
  public:
    Watch(): 
      hh(0, 2, Color(204, 102, 0), 0), 
      hl(2, 1, Color(255, 153, 51), 0), 
      mh(4, 2, Color(204, 102, 0), 0), 
      ml(6, 1, Color(255, 153, 51), 0) {}
    void update(int hour, int minute);
    void draw(Matrix matrix);
  private:
    WatchDigit hh;  
    WatchDigit hl;
    WatchDigit mh;
    WatchDigit ml;
};
