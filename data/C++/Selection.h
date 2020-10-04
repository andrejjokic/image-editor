#pragma once
#include <set>
#include <string>
#include <vector>
#include "Rectangle.h"
#include "Point.h"
#include "Pixel.h"


class Selection {
public:
	std::string getName()const { return name; }
	bool contains(Point point)const;
	bool isActive()const { return active; }
	void setActive(bool active) { this->active = active; }
protected:
	friend class Image;
	friend class XMLFormatter;
private:
	Selection(std::string name, const std::vector<Rectangle>& rectangles);
	Selection(std::string name, const std::set<Point>& points);
	std::string name;
	bool active = true;
	std::set<Point> collection;
};