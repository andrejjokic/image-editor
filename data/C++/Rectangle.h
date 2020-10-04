#pragma once
#include "Pixel.h"
#include "Exceptions.h"
#include "Point.h"
#include <vector>

class Rectangle {
public:
	Rectangle(const Point& p, int height, int width);
	int getHeight()const { return height; }
	int getWidth()const { return width; }
	void setHeight(int val) { height = val; }
	void setWidth(int val) { width = val; }
	void setUpperLeftPoint(const Point& p) { upperLeftPoint = p; }
	Point getUpperLeftPoint()const { return upperLeftPoint; }
	bool contains(const Rectangle& r) const;
	bool overlapsWith(const Rectangle& r) const;
	bool contains(const Point& p)const;
	std::vector<Point> getAllPoints()const;
private:
	Point upperLeftPoint;
	int height;
	int width;
};