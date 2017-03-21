#ifndef EDGE_H
#define EDGE_H

#include "point.h"

class Edge {
public:
    Edge(Point* start, Point* left, Point* right) : start(start), left(left), right(right) {
		neighbour = nullptr;
		end	= nullptr;
		k = (right->x - left->x) / (left->y - right->y) ;
		n = start->y - k * start->x ;
		direction = new Point(right->y - left->y, -(right->x - left->x));
	}

	~Edge() {
		delete direction;
	}
    
    Point* start;  // pointer to start point
    Point* end;  // pointer to end point
    Point* direction;  // directional vector, from "start", points to "end", normal of |left, right|
    Point* left;  // pointer to Voronoi place on the left side of edge
    Point* right;  // pointer to Voronoi place on the right side of edge
    double k, n;  // directional coeffitients satisfying equation y = k*x + n (edge lies on this line)
    Edge* neighbour;  // some edges consist of two parts, so we add the pointer to another part to connect them at the end of an algorithm
};

#endif  // EDGE_H