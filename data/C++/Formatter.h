#pragma once
#include <list>
#include <string>

class Pixel;

struct HeightWidth {

	HeightWidth(int height = 0,int width = 0) : height(height),width(width) {}
	int width;
	int height;
};

using ImageInfo = std::pair<HeightWidth, std::list<Pixel>*>;


//IF YOU USE FORMATTER FUNCTIONS,THEN YOU MUST ALSO DELETE LISTS
//path should look like this "C:\\Users\\BGB-HP-02\\Desktop\\fileName.bmp"
class Formatter {
public:
	virtual void write(ImageInfo p,std::string destination) =0;
	virtual ImageInfo read(std::string source) =0;	//creates list that you should later delete
};