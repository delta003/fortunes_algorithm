#ifndef PARABOLA_H
#define PARABOLA_H

#include "point.h"

class Event;
class Edge;

/*
	A class that stores information about an item in beachline sequence.
	It can represent an arch of parabola or an intersection between two archs (which defines an edge).
	
    Binary tree (internal nodes are edges, leaves are archs).
*/
class Parabola {
public:
	Parabola();  // empty for edge
	Parabola(Point* focus);  // with focus parameter for an arch

	void SetLeft(Parabola* p) {
        _left = p;
        p->parent = this;
    }
    
	void SetRight(Parabola* p) {
        _right = p;
        p->parent = this;
    }

	Parabola* Left() { return _left; }
	Parabola* Right() { return _right; }

	static Parabola* GetLeft(Parabola* p);  // returns the closest left leave of the tree
	static Parabola* GetRight(Parabola* p);  // returns the closest right leafe of the tree
	static Parabola* GetLeftParent(Parabola* p);  // returns the closest parent which is on the left
	static Parabola* GetRightParent(Parabola* p);  // returns the closest parent which is on the right
	static Parabola* GetLeftChild(Parabola* p);  // returns the closest leave which is on the left of current node
	static Parabola* GetRightChild(Parabola* p);  // returns the closest leave which is on the right of current node
    
    bool is_leaf;  // flag whether the node is Leaf or internal node
    Point* site;  // pointer to the focus point of parabola (when it is parabola)
    Edge* edge;  // pointer to the edge (when it is an edge)
    Event* circle_event;  // pointer to the event, when the arch disappears (circle event)
    Parabola* parent;  // pointer to the parent node in tree
	
private:
    Parabola* _left;
	Parabola* _right;
};

#endif  // PARABOLA_H
