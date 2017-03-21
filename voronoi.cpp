#include "voronoi.h"

#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>

#include "base/logger.h"

using base::Logger;
using std::min;
using std::max;
using std::list;
using std::to_string;
using std::sort;

Edges* Voronoi::GetEdges(Vertices* places) {
    clear();
    
    this->places = places;
    
    width = 0;
    height = 0;
    for (Vertices::iterator i = places->begin(); i != places->end(); i++) {
        width = max(width, (*i)->x);
        height = max(height, (*i)->y);
    }
    width *= 10;
    height *= 10;
    
    root = nullptr;
    edges = new Edges();
    for (Vertices::iterator i = places->begin(); i != places->end(); i++) {
		queue.push(new Event(*i, true));
    }
    Logger::instance()->OutputSuccess("Events created");
    
    Logger::instance()->OutputInfo("Starts iteration through events...");
	Event *e;
    int iteration = 0;
	while (!queue.empty()) {
        iteration++;
		e = queue.top();
		queue.pop();
		ly = e->point->y;
		if (deleted.find(e) != deleted.end()) {
            Logger::instance()->OutputInfo("Skipping iteration #" + to_string(iteration) + " (deleted event)");
            delete(e);
            deleted.erase(e);
            continue;
        }
        Logger::instance()->OutputInfo("Iteration #" + to_string(iteration));
		if (e->place_event) InsertParabola(e->point);
		else RemoveParabola(e);
		delete e;
	}
    Logger::instance()->OutputSuccess("Done #iterations = " + to_string(iteration));
	
    Logger::instance()->OutputInfo("Finishing edges...");
	FinishEdge(root);
	for (Edges::iterator i = edges->begin(); i != edges->end(); i++)
		if ((*i)->neighbour != nullptr) {
			(*i)->start = (*i)->neighbour->end;
			delete (*i)->neighbour;
		}
    Logger::instance()->OutputSuccess("Finishing edges completed");
	
	return edges;
}

void Voronoi::InsertParabola(Point *p) {
    Logger::instance()->OutputInfo("Inserting parabola");
    
	if (root == nullptr) {
        root = new Parabola(p);
        
        Logger::instance()->OutputSuccess("Parabola inserted");
        return;
    }

	if (root->is_leaf && root->site->y - p->y < 1) {
		Point* fp = root->site;
		root->is_leaf = false;
		root->SetLeft(new Parabola(fp));
		root->SetRight(new Parabola(p));
		Point* s = new Point((p->x + fp->x) / 2, height);
		points.push_back(s);
		if (p->x > fp->x) root->edge = new Edge(s, fp, p);
		else root->edge = new Edge(s, p, fp);
        edges->push_back(root->edge);
        
        Logger::instance()->OutputSuccess("Parabola inserted");
		return;
	}

	Parabola* par = GetParabolaByX(p->x);
	
	if (par->circle_event != nullptr) {
		deleted.insert(par->circle_event);
		par->circle_event = nullptr;
	}
	
	Point* start = new Point(p->x, GetY(par->site, p->x));
	points.push_back(start);

	Edge* el = new Edge(start, par->site, p);
	Edge* er = new Edge(start, p, par->site);

	el->neighbour = er;
	edges->push_back(el);

	par->edge = er;
	par->is_leaf = false;

	Parabola* p0 = new Parabola(par->site);
	Parabola* p1 = new Parabola(p);
	Parabola* p2 = new Parabola(par->site);

	par->SetRight(p2);
	par->SetLeft(new Parabola());
	par->Left()->edge = el;

	par->Left()->SetLeft(p0);
	par->Left()->SetRight(p1);
	
	CheckCircle(p0);
	CheckCircle(p2);
    
    Logger::instance()->OutputSuccess("Parabola inserted");
}

void Voronoi::RemoveParabola(Event* e) {
    Logger::instance()->OutputInfo("Removing parabola");
    
	Parabola* p1 = e->arch;
    
	Parabola* xl = Parabola::GetLeftParent(p1);
	Parabola* xr = Parabola::GetRightParent(p1);

	Parabola* p0 = Parabola::GetLeftChild(xl);
	Parabola* p2 = Parabola::GetRightChild(xr);

	if (p0->circle_event) {
        deleted.insert(p0->circle_event);
        p0->circle_event = nullptr;
    }
	if (p2->circle_event) {
        deleted.insert(p2->circle_event);
        p2->circle_event = nullptr;
    }

	Point* p = new Point(e->point->x, GetY(p1->site, e->point->x));
	points.push_back(p);

	xl->edge->end = p;
	xr->edge->end = p;
	
	Parabola* higher;
	Parabola* par = p1;
    
    while (par != root) {
		par = par->parent;
		if (par == xl) higher = xl;
		if (par == xr) higher = xr;
	}
	higher->edge = new Edge(p, p0->site, p2->site);
	edges->push_back(higher->edge);

	Parabola* gparent = p1->parent->parent;
	if (p1->parent->Left() == p1) {
		if (gparent->Left() == p1->parent) gparent->SetLeft(p1->parent->Right());
		if (gparent->Right() == p1->parent) gparent->SetRight(p1->parent->Right());
	} else {
		if (gparent->Left() == p1->parent) gparent->SetLeft(p1->parent->Left());
		if (gparent->Right() == p1->parent) gparent->SetRight(p1->parent->Left());
	}
    
	delete p1->parent;
	delete p1;

	CheckCircle(p0);
	CheckCircle(p2);
    
    Logger::instance()->OutputSuccess("Parabola removed");
}

void Voronoi::FinishEdge(Parabola* n) {
	if (n->is_leaf) {
        delete n;
        return;
    }
	
    double mx;
	if (n->edge->direction->x > 0.0) mx = max(width, n->edge->start->x + 10);
	else mx = min(0.0, n->edge->start->x - 10);
	
	Point* end = new Point(mx, mx * n->edge->k + n->edge->n);
	n->edge->end = end;
	points.push_back(end);
			
	FinishEdge(n->Left());
	FinishEdge(n->Right());
    
	delete n;
}

double Voronoi::GetXOfEdge(Parabola* par, double y) {
	Parabola* left = Parabola::GetLeftChild(par);
	Parabola* right = Parabola::GetRightChild(par);

	Point* p = left->site;
	Point* r = right->site;

	double dp = 2.0 * (p->y - y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * p->x / dp;
	double c1 = y + dp / 4 + p->x * p->x / dp;
			
    dp = 2.0 * (r->y - y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * r->x/dp;
	double c2 = ly + dp / 4 + r->x * r->x / dp;
			
	double a = a1 - a2;
	double b = b1 - b2;
	double c = c1 - c2;
			
	double disc = b*b - 4 * a * c;
	double x1 = (-b + sqrt(disc)) / (2 * a);
	double x2 = (-b - sqrt(disc)) / (2 * a);

	double ry;
	if (p->y < r->y ) ry = max(x1, x2);
	else ry = min(x1, x2);

	return ry;
}

Parabola* Voronoi::GetParabolaByX(double xx) {
	Parabola* par = root;
	double x = 0.0;
	while (!par->is_leaf) {
		x = GetXOfEdge(par, ly);
		if (x > xx) par = par->Left();
		else par = par->Right();				
	}
	return par;
}

double Voronoi::GetY(Point* p, double x) {
	double dp = 2 * (p->y - ly);
	double a1 = 1 / dp;
	double b1 = -2 * p->x / dp;
	double c1 = ly + dp / 4 + p->x * p->x / dp;
	return (a1 * x * x + b1 * x + c1);
}

void Voronoi::CheckCircle(Parabola* b) {
	Parabola* lp = Parabola::GetLeftParent(b);
	Parabola* rp = Parabola::GetRightParent(b);
	Parabola* a = Parabola::GetLeftChild(lp);
	Parabola* c = Parabola::GetRightChild(rp);
    
	if (a == nullptr || c == nullptr || a->site == c->site) return;

	Point* s = GetEdgeIntersection(lp->edge, rp->edge);
	if (s == nullptr) return;

	double dx = a->site->x - s->x;
	double dy = a->site->y - s->y;
	double d = std::sqrt(dx * dx + dy * dy);
    if (s->y - d >= ly) return;

	Event* e = new Event(new Point(s->x, s->y - d), false);
	points.push_back(e->point);
	b->circle_event = e;
	e->arch = b;
	queue.push(e);
}

Point* Voronoi::GetEdgeIntersection(Edge *a, Edge* b) {
	double x = (b->n - a->n) / (a->k - b->k);
	double y = a->k * x + a->n;
	if ((x - a->start->x) / a->direction->x < 0) return nullptr;
	if ((y - a->start->y) / a->direction->y < 0) return nullptr;
	if ((x - b->start->x) / b->direction->x < 0) return nullptr;
	if ((y - b->start->y) / b->direction->y < 0) return nullptr;
	Point* p = new Point(x, y);
    points.push_back(p);
	return p;
}

void Voronoi::clear() {
    Logger::instance()->OutputInfo("Clearing allocated memory...");
    while (!deleted.empty()) {
        Event* e = *(deleted.begin());
        deleted.erase(deleted.begin());
        if (e != nullptr) delete e;
    }
    for (list<Point*>::iterator i = points.begin(); i != points.end(); i++)
        if (*i != nullptr) delete (*i);
    while (!queue.empty()) {
        Event* e = queue.top();
        queue.pop();
        if (e != nullptr) delete e;
    }
    if (places != nullptr) {
        for (Vertices::iterator i = places->begin(); i != places->end(); i++)
            if (*i != nullptr) delete (*i);
        places->clear();
        delete places;
    }
    if (edges != nullptr) {
        for (Edges::iterator i = edges->begin(); i != edges->end(); i++)
            if (*i != nullptr) delete (*i);
        edges->clear();
        delete edges;
    }
    Logger::instance()->OutputSuccess("Memory freed");
}


	