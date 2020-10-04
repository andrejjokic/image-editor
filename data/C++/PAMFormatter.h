#pragma once
#include "Formatter.h"
#include <vector>

struct PAMHeader {

	PAMHeader(std::string magic = "", int w = 0, int h = 0, int d = 0, int max = 0, std::string typ = "") {
		magicNumber = magic; width = w;	height = h;	depth = d;	maxVal = max; type = typ;
	}
	std::string magicNumber;
	int width;
	int height;
	int depth;
	int maxVal;
	std::string type;
};

struct PAMBody {

	std::vector<unsigned char> body;
};

class PAMFormatter : public Formatter {
public:
	void write(ImageInfo image,std::string destination) override;
	ImageInfo read(std::string source)override;

private:
	void readHeader(std::ifstream& is,PAMHeader& pamHeader);
	void readBody(std::ifstream& is, PAMHeader& pamHeader,PAMBody& pamBody);
	void writeHeader(std::ofstream& os,PAMHeader& pamHeader);
	void writeBody(std::ofstream& os,PAMBody& pamBody);
	std::list<Pixel>* extractPixels(PAMHeader& pamHeader,PAMBody& pamBody);	//creates new list of pixels that must be deleted
	void putPixels(std::list<Pixel>* pixels, PAMHeader& pamHeader,PAMBody& pamBody);
	void prepareHeader(HeightWidth info,PAMHeader& pamHeader);

};

