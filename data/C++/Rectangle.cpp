#include "Rectangle.h"

Rectangle::Rectangle(const Point& p, int height, int width) {

	if (height <= 0 || width <= 0)
		throw UnappropriateSize();

	this->upperLeftPoint = p;
	this->height = height;
	this->width = width;
}

bool Rectangle::overlapsWith(const Rectangle& r) const {

	Point l1(upperLeftPoint);
	Point r1(upperLeftPoint.addX(width).addY(height));
	Point l2(r.upperLeftPoint);
	Point r2(r.upperLeftPoint.addX(width).addY(height));

	// If one rectangle is on left side of other 
	if (l1.getX() > r2.getX() || l2.getX() > r1.getX())
		return false;

	// If one rectangle is above other 
	if (l1.getY() < r2.getY() || l2.getY() < r1.getY())
		return false;

	return true;
}

bool Rectangle::contains(const Rectangle& r) const {
	Point nw1(upperLeftPoint), ne1(upperLeftPoint.addX(width));
	Point sw1(upperLeftPoint.addY(height)), se1(upperLeftPoint.addX(width).addY(height));
	Point nw2(r.upperLeftPoint), ne2(r.upperLeftPoint.addX(r.width));
	Point sw2(r.upperLeftPoint.addY(r.height)), se2(r.upperLeftPoint.addX(r.width).addY(r.height));

	return
		(nw1.getX() <= nw2.getX() && nw1.getY() <= nw2.getY()) &&
		(ne1.getX() >= ne2.getX() && ne1.getY() <= ne2.getY()) &&
		(sw1.getX() <= sw2.getX() && sw1.getY() >= sw2.getY()) &&
		(se1.getX() >= se2.getX() && se1.getY() >= se2.getY());
}

bool Rectangle::contains(const Point& p)const {

	bool xd = (p.getX() >= upperLeftPoint.getX()) && (p.getX() <= upperLeftPoint.addX(width).getX());
	bool yd = (p.getY() >= upperLeftPoint.getY()) && (p.getY() <= upperLeftPoint.addY(height).getY());

	return xd && yd;
}

std::vector<Point> Rectangle::getAllPoints()const {

	std::vector<Point> points;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			points.push_back(upperLeftPoint.addY(i).addX(j));

	return points;
}