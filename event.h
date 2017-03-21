#ifndef EVENT_H
#define EVENT_H

#include "point.h"
#include "parabola.h"

//	The class for storing place / circle event in event queue.
class Event {
public:
    Event(Point* point, bool place_event) : point(point), place_event(place_event) {
		y = point->y;
		arch = nullptr;
	}
    
    struct CompareEvent : public std::binary_function<Event*, Event*, bool>
	{
		bool operator() (const Event* l, const Event* r) const { return (l->y < r->y); }
	};
    
    bool place_event;  // whether it is a place event or not
    double y;  // y coordinate of "point", events are sorted by this "y"
    Point* point;  // the point at which current event occurs (top circle point for circle event, focus point for place event)
    Parabola* arch;  // if "place_event", it is an arch above which the event occurs
};

#endif  // EVENT_H
