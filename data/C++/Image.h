#pragma once
#include <vector>
#include <string>
#include <list>
#include <map>
#include "Selection.h"
#include <regex>

class Formatter;
class Layer;
class Selection;
class Rectangle;
class Pixel;
class Operation;
class Point;

enum OperationsIds { ADD, SUB, REV_SUB, MUL, DIV, REV_DIV, POW, LOG, ABS, MIN, MAX };

//U operacijama nad slikom ucestvuju svi aktivni slojevi uzimajuci u obzir aktivne selekcije.
//Ako nema aktivnih selekcija operacije se izvrsavaju nad svim pikselima

class Image {
public:
	Image();	//creates predefined operations
	~Image();	//delete all layers,selections,operations, will implement later
	Image& setLayerActivity(int num, bool active);
	Image& setLayerVisibility(int num, bool visible);
	Image& setLayerTransparency(int num, double transp);
	const Layer* operator[](int num)const;
	Image& addLayer(std::string source);
	Image& addLayer(int height, int width);
	Image& addLayer(Layer* lay);
	Image& addLayer();
	Image& removeLayer(int num);

	Image& addSelection(std::string name, const std::vector<Rectangle>& rectangles);
	Image& addSelection(Selection* sel);
	Image& setSelectionActivity(std::string name, bool active);
	Image& removeSelection(std::string name);

	Image& operate(std::string name);	//does operation with that name
	Image& addOperation(std::string name, Operation* op);
	Image& addOperation(Operation* op);

	void saveImage(std::string destination);
	void saveOperation(std::string operation,std::string destination);
	void saveProject(std::string destination);

	void loadProject(std::string source);
	std::string loadCompositeFunction(std::string source);
	void printSaveImageWarning();
	void printSaveProjectWarning();
	
	Operation* createBasicOperation(OperationsIds opId);
	Operation* createBasicOperation(OperationsIds opId, int x);
	Operation* createCompositeFunction();
	
private:
	void resizeAllLayers();
	void adjustLayers(Layer* l);
	Layer* operator[](int num);
	std::list<Pixel>* createImage()const;
	Selection* operator[](std::string name);
	bool isActivePixel(Point p)const;

	void loadSelections(std::string text);
	void loadLayers(std::string text);
	void loadOperations(std::string text);

	Layer* loadLayer(std::string text);
	Selection* loadSelection(std::string text);
	Operation* loadBasicOperation(std::string text);
	Operation* loadCompositeOperation(std::sregex_iterator& begin);

	std::vector<Layer*> layers;
	std::map<std::string, Selection*> selections;	
	std::map<std::string, Operation*> operations;
	int maxHeight = 0;
	int maxWidth = 0;
	int activeSelections = 0;
	bool imageSaved = true;		//moram da proverim svuda
	bool projectSaved = true;	//moram da proverim svuda
};