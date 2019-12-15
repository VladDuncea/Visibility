#include "Point2D.h"

Point2D::Point2D(double X, double Y) : X(X), Y(Y) {}

Point2D Point2D::operator=(const Point2D& Obj) {
	if (this == &Obj)
		return *this;
	X = Obj.X;
	Y = Obj.Y;
	return *this;
}

bool Point2D::operator==(const Point2D& Obj) const {
	return (Obj.X == X && Obj.Y == Y);
}

bool Point2D::operator<(const Point2D& Obj) const {
	if (X < Obj.X)
		return true;
	if (X == Obj.X)
		if (Y < Obj.Y)
			return true;
	return false;
}

bool Point2D::operator!=(const Point2D& Obj) const {
	return !(*this == Obj);
}

istream& operator>>(istream& in, Point2D& Obj) {
	in >> Obj.X >> Obj.Y;
	return in;
}

ostream& operator<<(ostream& out, const Point2D& Obj) {
	out << '(' << Obj.X << ", " << Obj.Y << ')';
	return out;
}

double orientation(const Point2D& A, const Point2D& B, const Point2D& C) {
	return (A.X * B.Y + B.X * C.Y + C.X * A.Y) - (A.Y * B.X + B.Y * C.X + C.Y * A.X);
}

bool isInside(const Point2D& A, const Point2D& B, const Point2D& C, const Point2D& P, bool allowOnEdge) {
	/* Calculate area of triangle ABC */
	double ABC = abs(orientation(A, B, C));

	// Calculate area of triangle PBC
	double PBC = abs(orientation(P, B, C));

	// Calculate area of triangle PAC 
	double APC = abs(orientation(A, P, C));

	// Calculate area of triangle PAB 
	double ABP = abs(orientation(A, B, P));

	//check if point is on edges
	if (!allowOnEdge && (ABP == 0 || APC == 0 || PBC == 0))
		return false;

	// Check if sum of A1, A2and A3 is same as A
	return (ABC == PBC + APC + ABP);
}

ostream& operator<<(ostream& out, const Triangle& Obj) {
	out << '{' << Obj.A << ", " << Obj.B << ", " << Obj.C << '}';
	return out;
}

Triangle::Triangle(const Point2D& A, const Point2D& B, const Point2D& C) : A(A), B(B), C(C) {}

bool Triangle::operator!=(const Triangle& Obj) const {
	return A != Obj.A || B != Obj.B || C != Obj.C;
}

bool Triangle::operator<(const Triangle& Obj) const {
	return abs(orientation(A, B, C)) < abs(orientation(Obj.A, Obj.B, Obj.C));
}

bool isAdjacent(const Triangle& A, const Triangle& B) {
	set<Point2D> Aux;
	Aux.insert({ A.A, A.B, A.C, B.A, B.B, B.C });
	return Aux.size() == 4;
}
