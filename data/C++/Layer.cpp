#include "Layer.h"
#include <regex>
#include "PAMFormatter.h"
#include "XMLFormatter.h"
#include "BMPFormatter.h"
#include "Exceptions.h"

//int Layer::maxHeight = 0;
//int Layer::maxWidth = 0;
//int Layer::numOfLayers = 0;
int Layer::ID = 0;

Layer::~Layer() {

	auto pom = image.second;
	delete pom;
}

Layer::Layer(int height, int width) {

	transparency = 1;	//not transparent layer if no starting image
	image = std::make_pair(HeightWidth(height, width), new std::list<Pixel>);

	Pixel transp(255, 255, 255,255);

	for (int i = 0; i < image.first.height * image.first.width; i++) {
		image.second->push_back(transp);
	}

	//maxHeight = (height > maxHeight) ? height : maxHeight;
	//maxWidth = (width > maxWidth) ? width : maxWidth;

	//increaseSize(height,width);
}


Layer::Layer(std::string source) {

	std::regex rx("[^.]*.([a-z]*)");
	std::smatch result;

	std::regex_match(source, result, rx);
	std::string format = result.str(1);

	Formatter* formatter = nullptr;

	if (format == "pam")
		formatter = new PAMFormatter();
	else if (format == "bmp")
		formatter = new BMPFormatter();
	else if (format == "xml")
		formatter = new XMLFormatter();
	else
		throw NotAppropriateFormat();

	image = formatter->read(source);

	//maxHeight = (height > maxHeight) ? height : maxHeight;
	//maxWidth = (width > maxWidth) ? width : maxWidth;

	//increaseSize();
}

void Layer::increaseSize(int maxHeight, int maxWidth) {

	increaseWidth(maxHeight, maxWidth);
	increaseHeight(maxHeight, maxWidth);
}

void Layer::increaseWidth(int maxHeight, int maxWidth) {

	if (image.first.width >= maxWidth)
		return;

	int diff = maxWidth - image.first.width;

	std::list<Pixel> padding;

	for (int i = 0; i < diff; i++) {
		padding.push_back(Pixel(255, 255, 255, 0));	//fully transparent pixel
	}


	for (int i = 0; i < image.first.height - 1; i++) {

		int offset = i * diff + (i + 1) * image.first.width;
		auto it = image.second->begin();
		std::advance(it, offset);

		image.second->insert(it, padding.begin(), padding.end());
	}

	for (auto pom : padding)
		image.second->push_back(pom);

	image.first.width = maxWidth;
}

void Layer::increaseHeight(int maxHeight, int maxWidth) {

	if (image.first.height >= maxHeight)
		return;

	int diff = maxHeight - image.first.height;

	for (int i = 0; i < diff * maxWidth; i++) {

		image.second->push_back(Pixel(255, 255, 255, 0));	//fully transparent pixel
	}

	image.first.height = maxHeight;
}
