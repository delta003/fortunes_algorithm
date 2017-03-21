#ifndef VORONOI_H
#define VORONOI_H

#include <list>
#include <queue>
#include <set>

#include "point.h"
#include "edge.h"
#include "parabola.h"
#include "event.h"

typedef std::list<Point*> Vertices;
typedef std::list<Edge*> Edges;

/* 
    Just one public method that takes list of points and returns list of edges in Voronoi diagram.
    Input points must have coordinates grater than zero.
*/
class Voronoi {
public:
    ~Voronoi() { clear(); }
    
    // Takes ownership of places
    Edges* GetEdges(Vertices* places);

private:
    void clear();  // clear all allocated memory, just in case something is missed
    
    void InsertParabola(Point* p);  // processing the place event
    void RemoveParabola(Event* e);  // processing the circle event
    void FinishEdge(Parabola* n);  // recursively finishes all infinite edges in the tree
    double GetXOfEdge(Parabola* par, double y);  // returns the current x position of an intersection point of left and right parabolas
    Parabola* GetParabolaByX(double xx);  // returns the Parabola that is under this "x" position in the current beachline
    double GetY(Point* p, double x);
    void CheckCircle(Parabola* b);  // checks the circle event (disappearing) of this parabola
    Point* GetEdgeIntersection(Edge* a, Edge* b);
    
    Vertices* places;  // container of places with which we work
    Edges* edges;  // container of edges which will be teh result
    double width, height;  // width of the diagram, height of the diagram
    Parabola* root;  // the root of the tree, that represents a beachline sequence
    double ly;  // current "y" position of the line (see Fortune's algorithm)
    
    std::set<Event*> deleted;  // set of deleted (false) Events (since we can not delete from PriorityQueue)
    std::list<Point*> points;  // list of all new points that were created during the algorithm
    std::priority_queue<Event*, std::vector<Event*>, Event::CompareEvent> queue;  // priority queue with events to process
};

#endif  // VORONOI_H
