#pragma once

class Color {
  public:
    Color(int r, int g, int b) {
      this->r = r;
      this->g = g;
      this->b = b;
    }
    int cR();
    int cG();
    int cB();
    void clear();
  private:
    int r;
    int g;
    int b;
};
