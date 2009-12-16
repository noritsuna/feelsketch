#include "PointMatrix.h"

namespace common {

  PointMatrix::PointMatrix(int dimension) :
  dimension_(dimension),
  points_((const int)0, (dimension*dimension*2)) {
  }

  PointMatrix::~PointMatrix() {

  }

  Ref<Point> PointMatrix::get(int x, int y) {
	int index = (dimension_ * y + x) * 2;
	Ref<Point> point(new Point(points_[index], points_[index+1]));
    return point;
  }

  void PointMatrix::set(int x, int y, int valX, int valY) {
	int index = (dimension_ * y + x) * 2;
    points_[index] = valX;
    points_[index+1] = valY;
  }

  int PointMatrix::getDimension() {
    return dimension_;
  }

  valarray<int> &PointMatrix::getPoints() {
    return points_;
  }

}
