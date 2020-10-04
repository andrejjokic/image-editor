#pragma once
#define PIXEL_MAX 255
#define PIXEL_MIN 0
#include <iostream>
class Pixel {

public:
	//default pixel is white and not transparent
	Pixel(int r = 255, int g = 255, int b = 255, int a = 255) :
		red(r), green(g), blue(b), alpha(a) {}

	friend std::ostream& operator<<(std::ostream& os , const Pixel& pix) {
		return os << "(" << pix.getRed() << "," << pix.getGreen() << "," << pix.getBlue() << "," << pix.getAlpha() << ")";
	}
	int getRed()const { return red; }
	int getGreen()const { return green; }
	int getBlue()const { return blue; }
	int getAlpha()const { return alpha; }
	void setRed(int x) { red = x; }
	void setGreen(int x) { green = x; }
	void setBlue(int x) { blue = x; }
	void setAlpha(int x) { alpha = x; }

private:
	int red;
	int green;
	int blue;
	int alpha;
};