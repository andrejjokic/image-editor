#pragma once
#include <string>
#include "Formatter.h"

class Selection;
class Layer;
class Operation;

class XMLFormatter : public Formatter {
public:
	virtual void write(ImageInfo p, std::string destination)override;
	virtual ImageInfo read(std::string source)override;
	std::string writeLayer(Layer* layer);
	std::string writeSelection(Selection* select);
	std::string writeOperation(Operation* op);
};