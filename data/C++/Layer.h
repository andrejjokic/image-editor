#pragma once
#include "Pixel.h"
#include <string>
#include <list>
#include "Formatter.h"

class Formatter;
class Image;

class Layer {
public:
	bool isActive()const { return active; }
	double getTransparency()const { return transparency; }
	bool isVisible()const { return visible; }
	int getWidth()const { return image.first.width; }
	int getHeight()const { return image.first.height; }
	int getId()const { return id; }
	//int getDepth()const { return depth; }

	void setActive(bool val) { active = val; }
	void setVisible(bool val) { visible = val; }
	void setTranspararency(double tr) { transparency = (tr > 1 || tr < 0) ? 1 : tr; }

protected:
	friend class Image;
	friend class XMLFormatter;
	Layer(std::string source);
	Layer(int height, int width);
	~Layer();
private:

	void increaseSize(int maxHeight, int maxWidth);
	void increaseWidth(int maxHeight, int maxWidth);
	void increaseHeight(int maxHeight, int maxWidth);

	ImageInfo image;
	//Formatter* formatter = nullptr;
	//int width = 0;
	//int height = 0;
	//int depth = ++numOfLayers;
	bool active = true;		//whether operations are done on this layer
	bool visible = true;	//whether it contributes in final picture
	double transparency = 1;	//range from 0-1(1 means it's not transparent)
	int id = ID++;
	static int ID;
	//static int numOfLayers;
	//static int maxWidth;
	//static int maxHeight;
};