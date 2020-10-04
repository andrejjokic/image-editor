#include "XMLFormatter.h"
#include "PAMFormatter.h"
#include "Exceptions.h"
#include "Layer.h"
#include "Operation.h"
#include "Selection.h"
#include <regex>
#include "Pixel.h"
#include <sstream>
#include <fstream>

void XMLFormatter::write(ImageInfo p, std::string destination) {

	if (destination.substr(destination.length() - 3) != "xml")
		throw NotAppropriateFormat();

	std::ofstream output(destination);

	if (!output)
		throw FileWontOpen();

	//this is somewhat of a header

	std::string text = "";

	text += "<Height>";
	text += std::to_string(p.first.height);
	text += "</Height>\n";

	text += "<Width>";
	text += std::to_string(p.first.width);
	text += "</Width>\n";

	text += "<Pixels>\n";

	for (auto pix : *p.second) {	//we write pixels in RGBA order

		text += std::to_string(pix.getRed());
		text += " ";
		text += std::to_string(pix.getGreen());
		text += " ";
		text += std::to_string(pix.getBlue());
		text += " ";
		text += std::to_string(pix.getAlpha());
		text += " ";
	}

	output << text;
	output << "\n</Pixel>";

	/*
	output << "<Height>" << p.first.height << "</Height>\n";
	output << "<Width>" << p.first.width << "</Width>\n";
	output << "<Pixels>\n";
	
	for (auto pix : *p.second) {	//we write pixels in RGBA order

		output << pix.getRed() << ' ';
		output << pix.getGreen() << ' ';
		output << pix.getBlue() << ' ';
		output << pix.getAlpha() << ' ';
	}

	output << "\n</Pixel>";
	*/
}

/*
ImageInfo XMLFormatter::read(std::string source) {

	if (source.substr(source.length() - 3) != "xml")
		throw NotAppropriateFormat();

	std::ifstream input(source);

	if (!input)
		throw NonExistentFile();

	int h;
	int w;
	std::list<Pixel>* pixels = new std::list<Pixel>;

	std::smatch result;
	std::regex rxH("<Height>([^<]+)</Height>");
	std::regex rxW("<Width>([^<]+)</Width>");

	std::string textLine;

	std::getline(input, textLine);			//getting height
	std::regex_match(textLine, result ,rxH);
	h = atoi(result.str(1).c_str());

	std::getline(input, textLine);			//getting width
	std::regex_match(textLine, result, rxW);
	w = atoi(result.str(1).c_str());

	std::getline(input, textLine);	//getting line <Pixel>
	
	for (int i = 0; i < w * h; i++) {	//getting pixels

		int r, g, b, a;

		input >> r;
		input.get();
		input >> g;
		input.get();
		input >> b;
		input.get();
		input >> a;
		input.get();

		pixels->push_back(Pixel(r, g, b, a));
	}

	return ImageInfo(HeightWidth(h, w), pixels);
}
*/

void addPixels(std::list<Pixel>* pixels,std::string text) {
	std::stringstream stream(text);

	while (1) {
		int r , g, b , a;
		stream >> r;
		if (!stream)
			break;
		stream >> g;
		stream >> b;
		stream >> a;

		pixels->push_back(Pixel(r, g, b, a));

	}
}

ImageInfo XMLFormatter::read(std::string source) {

	if (source.substr(source.length() - 3) != "xml")
		throw NotAppropriateFormat();

	std::ifstream input(source);

	if (!input)
		throw NonExistentFile();

	int h;
	int w;
	std::list<Pixel>* pixels = new std::list<Pixel>;

	std::smatch result;
	std::regex rxH("<Height>([^<]+)</Height>");
	std::regex rxW("<Width>([^<]+)</Width>");

	std::string textLine;

	std::getline(input, textLine);			//getting height
	std::regex_match(textLine, result, rxH);
	h = atoi(result.str(1).c_str());

	std::getline(input, textLine);			//getting width
	std::regex_match(textLine, result, rxW);
	w = atoi(result.str(1).c_str());

	std::getline(input, textLine);	//getting line <Pixel>

	/*
	for (int i = 0; i < w * h; i++) {	//getting pixels

		int r, g, b, a;

		input >> r;
		input.get();
		input >> g;
		input.get();
		input >> b;
		input.get();
		input >> a;
		input.get();

		pixels->push_back(Pixel(r, g, b, a));
	}*/

	std::string pixText;
	std::getline(input, pixText);

	addPixels(pixels, pixText);

	return ImageInfo(HeightWidth(h, w), pixels);
}

std::string XMLFormatter::writeLayer(Layer* layer) {

	std::string text = "";

	text += "<Layer>\n";

	text += "<active>";
	text += std::to_string(layer->isActive() ? 1 : 0);
	text += "</active>\n";

	text += "<visible>";
	text += std::to_string(layer->isVisible() ? 1 : 0);
	text += "</visible>\n";

	text += "<transparency>";
	text += std::to_string((int)(layer->getTransparency() * 100));	//we scale to integer 0-100
	text += "</transparency>\n";

	/*
	text += "<height>";
	text += std::to_string(layer->getHeight());
	text += "</height>\n";

	text += "<width>";
	text += std::to_string(layer->getWidth());
	text += "</width>\n";

	text += "<id>";
	text += std::to_string(layer->getId());
	text += "</id>\n";
	*/

	std::string fileName = "layer";
	fileName += std::to_string(layer->getId());
	fileName += ".xml";
	write(layer->image, fileName);

	text += "<path>";
	text += fileName;
	text += "</path>\n";

	text += "</Layer>\n";

	return text;
}

std::string XMLFormatter::writeSelection(Selection* select) {

	std::string text = "";

	text += "<Selection>\n";

	text += "<name>" + select->getName() + "</name>\n";

	text += "<active>";
	text += std::to_string(select->isActive() ? 1 : 0);
	text += "</active>\n";

	std::string fileName = select->getName() + ".xml";

	std::ofstream output(fileName, std::ofstream::out | std::ofstream::trunc);

	if (!output)
		throw FileWontOpen();

	int i = 0;
	std::string textToWrite = "";

	for (auto point : select->collection) {
		textToWrite += std::to_string(point.getX());
		textToWrite += ",";
		textToWrite += std::to_string(point.getY());
		i++;
		if (i < select->collection.size())
			textToWrite += " ";
	}

	output << textToWrite;

	text += "<Points>" + fileName + "</Points>\n";

	text += "</Selection>\n";

	return text;
}


std::string XMLFormatter::writeOperation(Operation* op) {

	return op->writeXML();
}

