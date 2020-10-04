#include "Selection.h"
#include <map>

Selection::Selection(std::string name, const std::vector<Rectangle>& rectangles) {
		
	this->name = name;

	for (auto rects : rectangles) {
		for (auto points : rects.getAllPoints()) {
			collection.insert(points);
		}
	}
}

Selection::Selection(std::string name, const std::set<Point>& points) {

	this->name = name;

	for (auto point : points) {
		collection.insert(point);
	}
}

bool Selection::contains(Point point)const {

	auto it = collection.find(point);

	if (it == collection.end())
		return false;

	return true;
}