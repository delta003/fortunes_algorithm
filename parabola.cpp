#include "parabola.h"
#include "point.h"

Parabola::Parabola() : site(nullptr), is_leaf(false), circle_event(nullptr), edge(nullptr), parent(nullptr) {}

Parabola::Parabola(Point *focus) : site(focus), is_leaf(true), circle_event(nullptr), edge(nullptr), parent(nullptr) {}

Parabola* Parabola::GetLeft(Parabola* p) {
	return GetLeftChild(GetLeftParent(p));
}

Parabola* Parabola::GetRight(Parabola* p) {
	return GetRightChild(GetRightParent(p));
}

Parabola* Parabola::GetLeftParent(Parabola* p) {
    if (p == nullptr) return nullptr;
	Parabola* par = p->parent;
	Parabola* pLast	= p;
	while (par->Left() == pLast) {
		if (!par->parent) return nullptr;
		pLast = par; 
		par = par->parent; 
	}
	return par;
}

Parabola* Parabola::GetRightParent(Parabola* p) {
    if (p == nullptr) return nullptr;
	Parabola* par = p->parent;
	Parabola* pLast	= p;
	while (par->Right() == pLast) {
		if (!par->parent) return nullptr;
		pLast = par;
        par = par->parent;
	}
	return par;
}

Parabola* Parabola::GetLeftChild(Parabola* p) {
	if (p == nullptr) return nullptr;
	Parabola* par = p->Left();
	while (!par->is_leaf) par = par->Right();
	return par;
}

Parabola* Parabola::GetRightChild(Parabola* p) {
    if (p == nullptr) return nullptr;
	Parabola* par = p->Right();
	while (!par->is_leaf) par = par->Left();
	return par;
}
