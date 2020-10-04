#include "BMPFormatter.h"
#include "Pixel.h"
#include <iostream>

ImageInfo BMPFormatter::read(std::string source) {

	if (source.substr(source.length() - 3) != "bmp")
		throw NotAppropriateFormat();

	std::cout << "Reading file...\n";

	BMP bmp;

	bmp.read(source);

	HeightWidth hw(bmp.bmp_info_header.height, bmp.bmp_info_header.width);

	auto pom = std::make_pair(hw, extractPixels(bmp));
	std::cout << "File has been successfully read!\n";

	return pom;
}

void BMPFormatter::write(ImageInfo p, std::string destination) {

	if (destination.substr(destination.length() - 3) != "bmp")
		throw NotAppropriateFormat();

	std::cout << "Writting file...\n";

	//prepareHeader(p.first);
	BMP bmp(p.first.width, p.first.height,true);
	putPixels(p.second,bmp);

	bmp.write(destination.c_str());

	std::cout << "File has been successfully written!\n";
}

std::list<Pixel>* BMPFormatter::extractPixels(BMP& bmp)const {

	std::list<Pixel>* pixels = new std::list<Pixel>;
	unsigned char B, G, R, A;
	/*
	for (auto pom = bmp.data.begin(); pom != bmp.data.end(); ) {

		B = *pom++;
		G = *pom++;
		R = *pom++;

		if (bmp.bmp_info_header.bit_count == 32) {
			A = *pom++;
		}
		else
			A = 255;

		pixels->push_back(Pixel(R, G, B, A));
	}
	*/
/*
	for (auto pom = bmp.data.rbegin(); pom != bmp.data.rend();) {
		B = *pom++;
		G = *pom++;
		R = *pom++;

		if (bmp.bmp_info_header.bit_count == 32) {
			A = *pom++;
		}
		else
			A = 255;

		pixels->push_back(Pixel(R, G, B, A));
	}
	*/

	int w = bmp.bmp_info_header.width;
	int h = bmp.bmp_info_header.height;
	int d = (bmp.bmp_info_header.bit_count == 32) ? 4 : 3;

	for (int i = h-1; i >= 0; i--) {
		for (int j = 0; j < w*d; j+=d) {
			R = bmp.data[i*w*d + j];
			G = bmp.data[i*w*d + j + 1];
			B = bmp.data[i*w*d + j + 2];
			if (d == 4)
				A = bmp.data[i*w*d + j + 3];
			else
				A = 255;

			pixels->push_back(Pixel(R, G, B, A));
		}
	}
	return pixels;
}

void BMPFormatter::putPixels(std::list<Pixel>* pixels,BMP& bmp) {

	//OVDE TREBA STARI ITERATOR NORMALAN
	bmp.data.clear();
	/*
	for (auto pom : (*pixels)) {

		bmp.data.push_back(pom.getBlue());
		bmp.data.push_back(pom.getGreen());
		bmp.data.push_back(pom.getRed());

		if (bmp.bmp_info_header.bit_count == 32)
			bmp.data.push_back(pom.getAlpha());
	}
	*/
	/*
	for (auto pomm = (*pixels).rbegin(); pomm != (*pixels).rend(); pomm++) {

		auto pom = *pomm;
		bmp.data.push_back(pom.getRed());
		bmp.data.push_back(pom.getGreen());
		bmp.data.push_back(pom.getBlue());
		if (bmp.bmp_info_header.bit_count == 32)
			bmp.data.push_back(pom.getAlpha());
	}
	*/
	
	int w = bmp.bmp_info_header.width;
	int h = bmp.bmp_info_header.height;
	int d = (bmp.bmp_info_header.bit_count == 32) ? 4 : 3;
	std::list<Pixel> finalOne;

	for (int i = 0; i < h; i++) {	
		
		std::list<Pixel> pom;

		for (int j = 0; j < w; j++) {
			Pixel tr = const_cast<Pixel&>(pixels->front());
			pom.push_back(tr);
			pixels->pop_front();
		}

		finalOne.insert(finalOne.begin(),pom.begin(), pom.end());
	}
	
	for (auto pomm = finalOne.begin(); pomm != finalOne.end(); pomm++) {

		auto pom = *pomm;
		bmp.data.push_back(pom.getRed());
		bmp.data.push_back(pom.getGreen());
		bmp.data.push_back(pom.getBlue());
		if (bmp.bmp_info_header.bit_count == 32)
			bmp.data.push_back(pom.getAlpha());
	}
}
