#ifndef __POINT_MATRIX_H__
#define __POINT_MATRIX_H__

#include "Counted.h"
#include <valarray>

namespace common {
  class Point : public Counted {
  private:
	int x_;
	int y_;

  public:
	Point(int x, int y) : x_(x), y_(y) { }
	~Point() { }
	int getX() { return x_; }
	int getY() { return y_; }
  };

  class PointMatrix : public Counted {
  private:

  public:
    int dimension_;
    valarray<int> points_;
    PointMatrix(int dimension);
    ~PointMatrix();
    Ref<Point> get(int x, int y);
    void set(int x, int y, int valX, int valY);
    int getDimension();
    valarray<int> &getPoints();
  };
}

#endif // __POINT_MATRIX_H__
