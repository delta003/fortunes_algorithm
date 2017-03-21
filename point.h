#ifndef POINT_H
#define POINT_H

struct Point {
    double x, y;
    
    Point(double x_, double y_) {
		x = x_;
		y = y_;
	}
};

#endif  // POINT_H