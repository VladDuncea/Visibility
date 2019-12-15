#pragma once
#include <iostream>
#include <set>

using namespace std;

class Point2D {
public:
	double X, Y;
	Point2D(double X = 0, double Y = 0);
	Point2D operator=(const Point2D& Obj);
	bool operator==(const Point2D& Obj) const;
	bool operator<(const Point2D& Obj) const;
	bool operator!=(const Point2D& Obj) const;

};

istream& operator>>(istream& in, Point2D& Obj);
ostream& operator<<(ostream& out, const Point2D& Obj);
double orientation(const Point2D& A, const Point2D& B, const Point2D& C);
bool isInside(const Point2D& A, const Point2D& B, const Point2D& C, const Point2D& P, bool allowOnEdge = false);

class Triangle {
public:
	Point2D A, B, C;
	Triangle(const Point2D& A = Point2D(), const Point2D& B = Point2D(), const Point2D& C = Point2D());
	bool operator!=(const Triangle& Obj) const;
	bool operator<(const Triangle& Obj) const;
};

bool isAdjacent(const Triangle& A, const Triangle& B);
ostream& operator<<(ostream& out, const Triangle& Obj);
