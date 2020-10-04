#pragma once
#include "Exceptions.h"

class Point {
public:
	Point(int xx = 0, int yy = 0) {
		if (xx < 0 || yy < 0)
			throw IndexOutOfBounds();
		x = xx;
		y = yy;
	}
	int getX()const { return x; }
	int getY()const { return y; }
	bool operator==(const Point& p)const {
		return (x == p.x) && (y == p.y);
	}
	
	Point mulX(int xx)const {
		return Point(x * xx, y);
	}

	Point mulY(int yy)const {
		return Point(x , y * yy);
	}

	Point addX(int xx)const {
		return Point(x + xx,y);
	}

	Point addY(int yy)const {
		return Point(x, y + yy);
	}

	bool operator<(const Point& p)const {		//this is needed because it is in a set
		
		return x < p.x || (x == p.x && y < p.y);
	}

	bool smallerX(const Point& p) const { return x < p.x; }
	bool smallerY(const Point& p) const { return y < p.y; }
private:
	int x;
	int y;
};