#include "Operation.h"
#include "Exceptions.h"
#include "Point.h"
#include "Image.h"
#include <algorithm>
#include <functional>
#include <cmath>
#include <vector>
#include <regex>

void Operation::correctPixel(Pixel& pixel)const {

	int Rt = pixel.getRed(), Gt = pixel.getGreen(), Bt = pixel.getBlue(), At = pixel.getAlpha();

	if (Rt < PIXEL_MIN)	Rt = PIXEL_MIN;	if (Rt > PIXEL_MAX) Rt = PIXEL_MAX;
	if (Gt < PIXEL_MIN)	Gt = PIXEL_MIN;	if (Gt > PIXEL_MAX) Gt = PIXEL_MAX;
	if (Bt < PIXEL_MIN)	Bt = PIXEL_MIN;	if (Bt > PIXEL_MAX) Bt = PIXEL_MAX;

	pixel.setRed(Rt);
	pixel.setGreen(Gt);
	pixel.setBlue(Bt);
}

void Operation::operator()(Pixel& pixel) {

	operate(pixel);
	
	correctPixel(pixel);
}
/*
void Operation::operator()(ImageInfo& image) {

	auto im = image.second; 

	std::for_each(im->begin(), im->end(), [this](Pixel& pixel) {
	
		this->operator()(pixel);

	});
}
*/

void Operation::operator()(ImageInfo& image) {

	operate(image);

	for (auto& pom : *image.second)
		correctPixel(pom);
}

void Add::operate(Pixel& pixel) {

	pixel.setRed(pixel.getRed() + x);
	pixel.setGreen(pixel.getGreen() + x);
	pixel.setBlue(pixel.getBlue() + x);
}

void Add::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Add::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(ADD) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

void Sub::operate(Pixel& pixel) {

	pixel.setRed(pixel.getRed() - x);
	pixel.setGreen(pixel.getGreen() - x);
	pixel.setBlue(pixel.getBlue() - x);
}

void Sub::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Sub::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(SUB) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

void RevSub::operate(Pixel& pixel) {

	pixel.setRed(x - pixel.getRed());
	pixel.setGreen(x - pixel.getGreen());
	pixel.setBlue(x - pixel.getBlue());
}

void RevSub::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string RevSub::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(REV_SUB) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

void Mul::operate(Pixel& pixel) {

	pixel.setRed(pixel.getRed() * x);
	pixel.setGreen(pixel.getGreen() * x);
	pixel.setBlue(pixel.getBlue() * x);
}

void Mul::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Mul::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(MUL) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

Div::Div(int x) : Operation("Division") {

	if (x == 0)
		throw NotAppropriateNumber();

	this->x = x;
}

void Div::operate(Pixel& pixel) {

	pixel.setRed(pixel.getRed() / x);
	pixel.setGreen(pixel.getGreen() / x);
	pixel.setBlue(pixel.getBlue() / x);
}

void Div::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Div::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(DIV) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

void RevDiv::operate(Pixel& pixel) {

	if (pixel.getRed() == 0 || pixel.getGreen() == 0 || pixel.getBlue() == 0)
		throw NotAppropriateNumber();

	pixel.setRed(x / pixel.getRed());
	pixel.setGreen(x / pixel.getGreen());
	pixel.setBlue(x / pixel.getBlue());
}

void RevDiv::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string RevDiv::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(REV_DIV) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

void Pow::operate(Pixel& pixel) {

	pixel.setRed(pow(pixel.getRed(),x));
	pixel.setGreen(pow(pixel.getGreen(),x));
	pixel.setBlue(pow(pixel.getBlue(),x));
}

void Pow::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Pow::writeXML() {

	/*std::string text = "";

	text += "<Operation>\n";
	text += "<operationId>" + std::to_string(POW) + "</operationId>\n";
	text += "<name>" + name + "</name>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;*/

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(POW) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

void Log::operate(Pixel& pixel) {

	if (pixel.getRed() < 0 || pixel.getBlue() < 0 || pixel.getGreen() < 0)
		throw NotAppropriateNumber();

	pixel.setRed((int)log(pixel.getRed()));
	pixel.setGreen((int)log(pixel.getGreen()));
	pixel.setBlue((int)log(pixel.getBlue()));
}

void Log::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Log::writeXML() {

	/*std::string text = "";

	text += "<Operation>\n";
	text += "<operationId>" + std::to_string(LOG) + "</operationId>\n";
	text += "<name>" + name + "</name>\n";
	text += "</Operation>\n";

	return text;*/

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(LOG) + "</operationId>\n";
	text += "</Operation>\n";

	return text;
}

void Abs::operate(Pixel& pixel) {

	pixel.setRed(abs(pixel.getRed()));
	pixel.setGreen(abs(pixel.getGreen()));
	pixel.setBlue(abs(pixel.getBlue()));
}

void Abs::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Abs::writeXML() {

	/*std::string text = "";

	text += "<Operation>\n";
	text += "<operationId>" + std::to_string(ABS) + "</operationId>\n";
	text += "<name>" + name + "</name>\n";
	text += "</Operation>\n";

	return text;*/

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(ABS) + "</operationId>\n";
	text += "</Operation>\n";

	return text;
}

void Min::operate(Pixel& pixel) {

	pixel.setRed((pixel.getRed() > x) ? x : pixel.getRed());
	pixel.setGreen((pixel.getGreen() > x) ? x : pixel.getGreen());
	pixel.setBlue((pixel.getBlue() > x) ? x : pixel.getBlue());
}

void Min::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Min::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(MIN) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

void Max::operate(Pixel& pixel) {

	pixel.setRed((pixel.getRed() < x) ? x : pixel.getRed());
	pixel.setGreen((pixel.getGreen() < x) ? x : pixel.getGreen());
	pixel.setBlue((pixel.getBlue() < x) ? x : pixel.getBlue());
}

void Max::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Max::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "<operationId>" + std::to_string(MAX) + "</operationId>\n";
	text += "<argument>" + std::to_string(x) + "</argument>\n";
	text += "</Operation>\n";

	return text;
}

void Inverse::operate(Pixel& pixel) {

	pixel.setRed(PIXEL_MAX - pixel.getRed());
	pixel.setGreen(PIXEL_MAX - pixel.getGreen());
	pixel.setBlue(PIXEL_MAX - pixel.getBlue());
}

void Inverse::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Inverse::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "</Operation>\n";

	return text;
}

void Gray::operate(Pixel& pixel) {

	int val = (pixel.getRed() + pixel.getGreen() + pixel.getBlue()) / 3;

	pixel.setRed(val);
	pixel.setGreen(val);
	pixel.setBlue(val);
}

void Gray::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string Gray::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "</Operation>\n";

	return text;
}


void BlackWhite::operate(Pixel& pixel) {

	Operation* op = new Gray();
	(*op)(pixel);
	delete op;

	int val = (pixel.getRed() > 127) ? PIXEL_MAX : PIXEL_MIN;

	pixel.setRed(val);
	pixel.setGreen(val);
	pixel.setBlue(val);
}


void BlackWhite::operate(ImageInfo& image) {

	for (auto& pom : *image.second)
		operate(pom);
}

std::string BlackWhite::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "</Operation>\n";

	return text;
}

CompositeFunction::CompositeFunction(std::string name, std::list<Operation*>* operations) : Operation(name) {

	this->operations = operations;
}

CompositeFunction::~CompositeFunction() {

	for (auto pom : *operations) {
		
		delete pom;
	}

	delete operations;
}

void CompositeFunction::operate(Pixel& pixel) {

	std::for_each(operations->begin(), operations->end(), [&pixel](Operation* op) {
	
		op->operate(pixel);
	});
}


void CompositeFunction::operate(ImageInfo& image) {

	std::for_each(operations->begin(), operations->end(), [&image](Operation* op) {

		op->operate(image);
	});
}

std::string CompositeFunction::writeXML() {

	std::string text = "";

	text += "<Operation>\n";

	text += "<name>" + name + "</name>\n";

	text += "<size>" + std::to_string(operations->size()) + "</size>\n";

	text += "</Operation>\n";

	for (auto pom : *operations) {
		text += pom->writeXML();
	}

	return text;
}

void Median::operate(Pixel& pixel) {}

void Median::operate(ImageInfo& image) {

	//std::list<Pixel> pixs;
	std::vector<Pixel> pom;
	//pixs.resize(image.first.height * image.first.width);
	for (auto s : *image.second) {
		//pixs.push_back(Pixel(s));
		pom.push_back(Pixel(s));
	}

	int i = 0;
	int w = image.first.width;
	int h = image.first.height;

	// x = i % width
	// y = i / width

	std::function<int(std::vector<int>&)> px = [](std::vector<int>& nums) {		//calculates median of an array
		
		if (nums.size() == 0)
			return 0;

		std::sort(nums.begin(),nums.end());
	
		int sr = nums.size() / 2;

		if (nums.size() % 1 == 0) {
			
			int pr = nums.size() / 2 - 1;

			sr = (nums[sr] + nums[pr]) / 2;
		}
		else
			sr = nums[sr];

		return sr;
	};

	//calculates median for all components
	std::function<Pixel(std::vector<Pixel>& image,int height,int width, std::vector<Point>&)> f =
		[px](std::vector<Pixel>& pix,int height,int width,std::vector<Point>& points) {
	
		int w = width;
		int h = height;


		std::vector<int> reds;
		std::vector<int> greens;
		std::vector<int> blues;

		for (int i = 0; i < points.size(); i++) {
				
			int offs = points[i].getY() * w + points[i].getX();
			
			Pixel& it = pix[offs];
				
			reds.push_back(it.getRed());
			greens.push_back(it.getGreen());
			blues.push_back(it.getBlue());

		}

		return Pixel(px(reds), px(greens), px(blues));
		
	};

	auto it = image.second->begin();

	for (auto pix : pom) {

		int x = i % w;
		int y = i / w;

		std::vector<Point> points;

		points.push_back(Point(x, y));	//this pixel
		if (x > 0)	//west pixel
			points.push_back(Point(x - 1, y));
		if (x < w - 1)	//east pixel
			points.push_back(Point(x + 1, y));
		if (y > 0)	//north pixel
			points.push_back(Point(x, y - 1));
		if (y < h - 1)	//south pixel
			points.push_back(Point(x, y + 1));

		Pixel trPix = f(pom, h, w, points);

		it->setRed(trPix.getRed());
		it->setGreen(trPix.getGreen());
		it->setBlue(trPix.getBlue());

		it++;
		i++;
	}
}

std::string Median::writeXML() {

	std::string text = "";

	text += "<Operation>\n";
	text += "<name>" + name + "</name>\n";
	text += "</Operation>\n";

	return text;
}