#include "PAMFormatter.h"
#include "Pixel.h"
#include <fstream>
#include <regex>
#include "Exceptions.h"
#include <iostream>


ImageInfo PAMFormatter::read(std::string source) {

	if (source.substr(source.length() - 3) != "pam")
		throw NotAppropriateFormat();

	std::ifstream input(source,std::ios_base::binary);

	if (!input)
		throw NonExistentFile();

	std::cout << "Reading file...\n";

	PAMHeader pamHeader;
	PAMBody pamBody;

	readHeader(input,pamHeader);
	readBody(input,pamHeader,pamBody);

	auto pom = std::make_pair(HeightWidth(pamHeader.height, pamHeader.width), extractPixels(pamHeader,pamBody));
	std::cout << "File has been successfully read!\n";
	return pom;
}

void PAMFormatter::readHeader(std::ifstream& is,PAMHeader& pamHeader) {

	std::string textLine;
	std::string magicNumber;
	int width;
	int height;
	int depth;
	int maxVal;
	std::string type;

	std::regex rx("[^ ]* (.*)");
	std::smatch result;

	std::getline(is, magicNumber);	//reading magic number

	std::getline(is, textLine);
	std::regex_match(textLine, result, rx);	//reading WIDTH
	width = atoi(result.str(1).c_str());

	std::getline(is, textLine);
	std::regex_match(textLine, result, rx);	//reading HEIGHT
	height = atoi(result.str(1).c_str());

	std::getline(is, textLine);
	std::regex_match(textLine, result, rx);	//reading DEPTH
	depth = atoi(result.str(1).c_str());

	std::getline(is, textLine);
	std::regex_match(textLine, result, rx);	//reading MAXVAL
	maxVal = atoi(result.str(1).c_str());

	std::getline(is, textLine);
	std::regex_match(textLine, result, rx);	//reading TYPE
	type = result.str(1);

	std::getline(is, textLine);	//reading ENDHDR

	pamHeader = PAMHeader(magicNumber, width, height, depth, maxVal, type);
}

void PAMFormatter::readBody(std::ifstream& is,PAMHeader& pamHeader,PAMBody& pamBody) {

	pamBody.body.resize(pamHeader.width * pamHeader.height * pamHeader.depth);

	is.read((char*)pamBody.body.data(), pamBody.body.size());
}

std::list<Pixel>* PAMFormatter::extractPixels(PAMHeader& pamHeader,PAMBody& pamBody) {

	std::list<Pixel>* pixels = new std::list<Pixel>;

	for (auto pom = pamBody.body.begin(); pom != pamBody.body.end(); pom += pamHeader.depth) {

		switch (pamHeader.depth) {
		case 1:
			pixels->push_back(Pixel(*pom, *pom, *pom));
			break;
		case 2:
			pixels->push_back(Pixel(*pom, *pom, *pom, *(pom + 1)));
			break;
		case 3:
			pixels->push_back(Pixel(*(pom + 2), *(pom + 1), *pom));
			break;
		case 4:
			pixels->push_back(Pixel(*(pom + 2), *(pom + 1), *pom, *(pom + 3)));
			break;
		}
	}

	return pixels;
}

void PAMFormatter::putPixels(std::list<Pixel>* pixels, PAMHeader& pamHeader,PAMBody& pamBody) {

	pamBody.body.clear();

	for (auto pom : *pixels) {

		switch (pamHeader.depth) {

		case 1:
			pamBody.body.push_back(pom.getBlue());
			break;
		case 2:
			pamBody.body.push_back(pom.getBlue());
			pamBody.body.push_back(pom.getAlpha());
			break;
		case 3:
			pamBody.body.push_back(pom.getBlue());
			pamBody.body.push_back(pom.getGreen());
			pamBody.body.push_back(pom.getRed());
			break;
		case 4:
			pamBody.body.push_back(pom.getBlue());
			pamBody.body.push_back(pom.getGreen());
			pamBody.body.push_back(pom.getRed());
			pamBody.body.push_back(pom.getAlpha());
			break;
		}
	}
}

void PAMFormatter::write(ImageInfo image,std::string destination) {

	if (destination.substr(destination.length() - 3) != "pam")
		throw NotAppropriateFormat();

	std::ofstream output(destination,std::ios_base::binary);

	if (!output)
		throw FileWontOpen();

	std::cout << "Writting file...\n";
	PAMHeader pamHeader;
	PAMBody pamBody;

	prepareHeader(image.first,pamHeader);
	putPixels(image.second,pamHeader,pamBody);
	writeHeader(output,pamHeader);
	writeBody(output,pamBody);

	std::cout << "File has been successfully written!\n";
}

void PAMFormatter::writeHeader(std::ofstream& output,PAMHeader& pamHeader) {

	output << pamHeader.magicNumber << std::endl;	//writing magic number

	output << "WIDTH " << pamHeader.width << std::endl;		//writing width

	output << "HEIGHT " << pamHeader.height << std::endl;	//writing height

	output << "DEPTH " << pamHeader.depth << std::endl;		//writing depth

	output << "MAXVAL " << pamHeader.maxVal << std::endl;	//writing maxVal

	output << "TUPLTYPE " << pamHeader.type << std::endl;	//writing tupltype

	output << "ENDHDR" << std::endl;	//writing ENDHDR
}

void PAMFormatter::writeBody(std::ofstream& output,PAMBody& pamBody) {

	output.write((const char*)pamBody.body.data(), pamBody.body.size());
}

void PAMFormatter::prepareHeader(HeightWidth info,PAMHeader& pamHeader) {

	std::string magicNumber = "P7";		//default
	int width = info.width;
	int height = info.height;
	int depth = 4;	//default
	int maxVal = 255;	//default for RGB
	std::string type = "RGB_ALPHA";	//default

	pamHeader = PAMHeader(magicNumber,width,height, depth, maxVal, type);
}