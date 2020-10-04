#include "Image.h"
#include "Exceptions.h"
#include "Layer.h"
#include "Pixel.h"
#include <sstream>
#include "Selection.h"
#include <functional>
#include <regex>
#include <algorithm>
#include <sstream>
#include "PAMFormatter.h"
#include "XMLFormatter.h"
#include "BMPFormatter.h"
#include "Operation.h"


Image::Image() {

	operations.insert(std::make_pair("Inverse", new Inverse()));
	operations.insert(std::make_pair("Gray", new Gray()));
	operations.insert(std::make_pair("BlackWhite", new BlackWhite()));
	operations.insert(std::make_pair("Median", new Median()));
}

Image::~Image() {

	std::for_each(layers.begin(), layers.end(), [](Layer* lay) {
		delete lay;
	});

	std::for_each(selections.begin(), selections.end(), [](std::pair<std::string,Selection*> par) {
		delete par.second;
	});

	std::for_each(operations.begin(), operations.end(), [](std::pair<std::string,Operation*> op) {
		delete op.second;
	});
}

Image& Image::addLayer(Layer* lay) {

	layers.push_back(lay);

	adjustLayers(lay);

	imageSaved = false;
	projectSaved = false;

	return *this;
}


Image& Image::addLayer(std::string source) {

	Layer* newLayer = new Layer(source);
	layers.push_back(newLayer);

	adjustLayers(newLayer);

	imageSaved = false;
	projectSaved = false;

	return *this;
}

Image& Image::addLayer(int height, int width) {

	if (height <= 0 || width <= 0)
		throw UnappropriateSize();

	Layer* newLayer = new Layer(height, width);
	layers.push_back(newLayer);
	adjustLayers(newLayer);

	imageSaved = false;
	projectSaved = false;

	return *this;
}

Image& Image::addLayer() {

	if (layers.size() == 0)
		throw FirstLayerNoDimension();

	Layer* newLayer = new Layer(maxHeight, maxWidth);
	layers.push_back(newLayer);

	imageSaved = false;
	projectSaved = false;

	return *this;
}

void Image::adjustLayers(Layer* newLayer) {

	if ((maxHeight != newLayer->getHeight()) || (maxWidth != newLayer->getWidth())) {

		maxHeight = (newLayer->getHeight() > maxHeight) ? newLayer->getHeight() : maxHeight;
		maxWidth = (newLayer->getWidth() > maxWidth) ? newLayer->getWidth() : maxWidth;

		resizeAllLayers();
	}
}

void Image::resizeAllLayers() {

	for (auto l : layers) {
		l->increaseSize(maxHeight, maxWidth);
	}
}

const Layer* Image::operator[](int num)const {

	return const_cast<Image&>(*this)[num];
}

Layer* Image::operator[](int num) {

	if (num < 0 || num >= (int)layers.size())
		throw IndexOutOfBounds();

	return layers[num];
}

Image& Image::setLayerActivity(int num, bool active) {

	(*this)[num]->setActive(active);

	projectSaved = false;
	
	return *this;
}

Image& Image::setLayerVisibility(int num, bool visible) {

	(*this)[num]->setVisible(visible);

	imageSaved = false;
	projectSaved = false;

	return *this;
}

Image& Image::setLayerTransparency(int num, double transp) {

	(*this)[num]->setTranspararency(transp);

	imageSaved = false;
	projectSaved = false;

	return *this;
}

Image& Image::removeLayer(int num) {

	Layer* oldOne = (*this)[num];

	layers.erase(layers.begin() + num);
	delete oldOne;

	imageSaved = false;
	projectSaved = false;

	return *this;
}

std::list<Pixel>* Image::createImage()const {

	if (layers.size() == 0)
		throw NoLayers();

	bool has = false;
	for (auto lay : layers) {
		if (lay->isVisible()) {
			has = true;
			break;
		}
	}
	if (!has)
		throw NoLayers();

	std::list<Pixel>* image = new std::list<Pixel>;
	image->resize(maxWidth * maxHeight);

	int round = 0;
	double charged = 0;

	for (auto lay : layers) {

		if (!lay->isVisible())
			continue;

		if (charged >= 1)
			break;

		if (round == 0) {					//it's first layer so we just copy pixels to our image
			auto it = image->begin();
			double proc = lay->getTransparency() * (1 - charged);
			for (auto pix : *lay->image.second) {
				*it++ = Pixel(pix.getRed(), pix.getGreen() , pix.getBlue() , pix.getAlpha() * proc);
			}
			charged += proc;
			round++;
			continue;
		}

		double proc = lay->getTransparency() * (1 - charged);
		auto it = image->begin();

		for (auto pix : *lay->image.second) {

			//A = A0 + (1-A0)*A1
			//R = R0*A0/A + R1 * (1-A0)*A1/A
			double itAlpha = it->getAlpha() / 255.;
			double pixAlpha = pix.getAlpha() / 255. * lay->getTransparency();

			double At = itAlpha + (1 - itAlpha) * pixAlpha;
			int Rt = it->getRed() * itAlpha / At + pix.getRed() * (1 - itAlpha) * pixAlpha / At;
			int Gt = it->getGreen() * itAlpha / At + pix.getGreen() * (1 - itAlpha) * pixAlpha / At;
			int Bt = it->getBlue() * itAlpha / At + pix.getBlue() * (1 - itAlpha) * pixAlpha / At;

			*it++ = Pixel(Rt, Gt, Bt, At * 255);
		}

		charged += proc;
		round++;
	}

	return image;
}

void Image::saveImage(std::string dest) {

	std::string format = dest.substr(dest.length() - 3);
	Formatter* formatter = 0;

	if (format == "pam")
		formatter = new PAMFormatter();
	else if (format == "bmp")
		formatter = new BMPFormatter();
	else if (format == "xml")
		formatter = new XMLFormatter();
	else
		throw NotAppropriateFormat();

	std::list<Pixel>* pixels = createImage();
	ImageInfo image = ImageInfo(HeightWidth(maxHeight, maxWidth), pixels);

	formatter->write(image, dest);
	delete pixels;

	imageSaved = true;
}

void Image::printSaveImageWarning() {

	if (!imageSaved)
		std::cout << "\n!!! IMAGE HAS NOT BEEN SAVED !!!\n";
}

void Image::printSaveProjectWarning() {

	if (!projectSaved)
		std::cout << "\n!!! PROJECT HAS NOT BEEN SAVED !!!\n\n";
}

Selection* Image::operator[](std::string name) {

	auto it = selections.find(name);

	if (it == selections.end())
		throw SelectionDoesntExist();

	return it->second;
}

Image& Image::addSelection(std::string name, const std::vector<Rectangle>& rectangles) {

	selections.insert(std::make_pair(name, new Selection(name, rectangles)));

	activeSelections++;

	projectSaved = false;

	return *this;
}

Image& Image::addSelection(Selection* sel) {

	selections.insert(std::make_pair(sel->getName(), sel));

	if (sel->isActive())
		activeSelections++;

	projectSaved = false;

	return *this;
}

Image& Image::setSelectionActivity(std::string name, bool active) {

	auto it = selections.find(name);

	if (it->second->isActive() && !active)
		activeSelections--;

	if (!it->second->isActive() && active)
		activeSelections++;

	it->second->setActive(active);

	projectSaved = false;

	return *this;
}

Image& Image::removeSelection(std::string name) {

	auto it = selections.find(name);

	Selection* oldOne = it->second;

	selections.erase(it);

	if (oldOne->isActive())
		activeSelections--;

	delete oldOne;

	projectSaved = false;

	return *this;
}

bool Image::isActivePixel(Point p)const {

	if (!activeSelections)		//if there is no active selection,operation should be done on all pixels
		return true;

	return std::count_if(selections.begin(), selections.end(), [p](std::pair<std::string,Selection*> par) {	

		return par.second->isActive() && par.second->contains(p);
	});
}

Image& Image::addOperation(std::string name, Operation* oper) {

	operations.insert(std::make_pair(name, oper));

	projectSaved = false;

	return *this;
}

Image& Image::addOperation(Operation* op) {

	operations.insert(std::make_pair(op->getName(), op));

	projectSaved = false;

	return *this;
}

Image& Image::operate(std::string name) {

	auto it = operations.find(name);

	if (it == operations.end())
		throw NoSuchOperation();

	// x = i % width
	// y = i / width

	std::cout << "Applying operation...\n";

	Operation* op = it->second;

	for (auto lay : layers) {

		if (!lay->isActive())
			continue;

		auto pic = lay->image;

		std::list<Pixel> temp;

		for (auto x : *pic.second)
			temp.push_back(Pixel(x));

		ImageInfo im = std::make_pair(HeightWidth(pic.first.height, pic.first.width), &temp);

		(*op)(im);

		int i = 0;
		auto its = temp.begin();
		for (auto& x : *pic.second) {

			Point p(i % maxWidth, i / maxWidth);

			if (isActivePixel(p))
				x = *its;

			its++;
			i++;
		}
	}

	std::cout << "Operation applied!\n";

	imageSaved = false;
	projectSaved = false;

	return *this;
}

void Image::saveOperation(std::string op, std::string dest) {

	auto it = operations.find(op);

	if (it == operations.end())
		throw NoSuchOperation();

	XMLFormatter xml;

	std::ofstream output(dest);

	if (!output)
		throw FileWontOpen();

	std::cout << "Saving operation...\n";

	std::string text = xml.writeOperation(it->second);

	output << text;

	std::cout << "Operation saved!\n";
}

void Image::saveProject(std::string destination) {

	std::string text = "";
	XMLFormatter xml;

	text += "<Image>\n";
	
	//writting layers
	text += "<Layers>\n";

	for (auto lay : layers) {
		text += xml.writeLayer(lay);
	}

	text += "</Layers>\n";
	//done writting layers

	//writting selections
	text += "<Selections>\n";

	for (auto sel : selections) {
		text += xml.writeSelection(sel.second);
	}

	text += "</Selections>\n";
	//done writting selections

	//writting operations
	text += "<Operations>\n";

	for (auto op : operations) {
		if (op.second->getName() == "Addition" || op.second->getName() == "Subtraction" || op.second->getName() == "Reverse Subtraction"
			|| op.second->getName() == "Multiplication" || op.second->getName() == "Division" || op.second->getName() == "Reverse Division"
			|| op.second->getName() == "Power" || op.second->getName() == "Logarithm" || op.second->getName() == "Absolute value" ||
			op.second->getName() == "Minimum" || op.second->getName() == "Maximum")
		
			continue;

		text += xml.writeOperation(op.second);
	}

	text += "</Operations>\n";
	//done writting operations

	//writting maxHeight and maxWidth
	text += "<maxHeight>" + std::to_string(maxHeight) + "</maxHeight>\n";
	text += "<maxWidth>" + std::to_string(maxWidth) + "</maxWidth>\n";
	//done writting maxHeight and maxWidth

	//writting number of active selections
	text += "<activeSelections>" + std::to_string(activeSelections) + "</activeSelections>\n";
	//done writting number of active selections

	text += "</Image>\n";

	std::ofstream output(destination);
	
	if (!output)
		throw FileWontOpen();

	std::cout << "Saving project...\n";

	output << text;
	
	std::cout << "Project saved!\n";

	projectSaved = true;
}

std::string Image::loadCompositeFunction(std::string src) {

	std::ifstream input(src);
	if (!input)
		throw NonExistentFile();

	std::stringstream buffer;
	buffer << input.rdbuf();
	std::string text = buffer.str();

	text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());	//removing newlines
	
	std::regex rx("<Operation>(.*?)</Operation>");

	std::sregex_iterator begin(text.begin(), text.end(), rx);

	Operation* op = loadCompositeOperation(begin);

	addOperation(op);

	projectSaved = false;

	return op->getName();
}

void Image::loadProject(std::string source) {

	if (source.substr(source.length() - 3) != "xml")
		throw NotAppropriateFormat();

	std::cout << "Loading project...\n";

	std::ifstream t(source);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string text = buffer.str();

	text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());	//removing newlines

	loadLayers(text);

	loadSelections(text);

	loadOperations(text);

	std::cout << "Project loaded!\n";
}

Layer* Image::loadLayer(std::string text) {

	//making regex
	std::regex rxActive("<active>([0-1])</active>");
	std::smatch activeResult;

	std::regex rxVisible("<visible>([0-1])</visible>");
	std::smatch visibleResult;

	std::regex rxTransp("<transparency>([0-9]*)</transparency>");
	std::smatch transpResult;

	std::regex rxPath("<path>(.*?)</path>");
	std::smatch pathResult;

	//matching information from string
	std::regex_search(text, activeResult, rxActive);
	std::regex_search(text, visibleResult, rxVisible);
	std::regex_search(text, transpResult, rxTransp);
	std::regex_search(text, pathResult, rxPath);

	//extracting information
	int activeI = atoi(activeResult.str(1).c_str());
	bool active = activeI ? true : false;
	int visibleI = atoi(visibleResult.str(1).c_str());
	bool visible = visibleI ? true : false;
	double transp = atoi(transpResult.str(1).c_str()) / 100.;
	std::string path = pathResult.str(1);

	Layer* lay = new Layer(path);
	lay->setActive(active);
	lay->setVisible(visible);
	lay->setTranspararency(transp);

	return lay;
}

/*
Selection* Image::loadSelection(std::string text) {

	//making regex
	std::regex rxName("<name>(.*?)</name>");
	std::smatch nameResult;

	std::regex rxActive("<active>([0-1])</active>");
	std::smatch activeResult;

	std::regex rxPoints("<Points>(.*?)</Points>");
	std::smatch pointsResult;

	std::regex rxPoint("([0-9]+),([0-9]+)");

	//matching begins
	std::regex_search(text, nameResult, rxName);
	std::regex_search(text, activeResult, rxActive);

	//matching all points
	std::regex_search(text, pointsResult, rxPoints);
	std::string path = pointsResult.str(1);

	std::ifstream input(path);

	if (!input)
		throw NonExistentFile();

	std::stringstream buffer;
	buffer << input.rdbuf();
	std::string allPoints = buffer.str();

	//std::string allPoints = pointsResult.str(1);

	std::sregex_iterator beginP(allPoints.begin(), allPoints.end(), rxPoint);
	std::sregex_iterator endP;

	std::set<Point> points;

	while (beginP != endP) {
		std::smatch pointRes = *beginP++;

		int x = atoi(pointRes.str(1).c_str());
		int y = atoi(pointRes.str(2).c_str());

		points.insert(Point(x, y));
	}

	std::string name = nameResult.str(1);
	bool active = (bool)atoi(activeResult.str(1).c_str());

	Selection* sel = new Selection(name, points);
	sel->setActive(active);

	return sel;
}
*/

/*
void addPoints(std::string text, std::set<Point>& points) {
	int i = 0;
	std::string x = "", y = "";
	int size = (int)text.length();

	while (i < size) {
		while (i < size && text[i] != ',') {
			x += text[i++];
		}

		i++;	//read ,

		while (i < size && text[i] != ' ' && text[i] != '\n') {
			y += text[i++];
		}

		i++;	//read space

		int xx = atoi(x.c_str());
		int yy = atoi(y.c_str());

		points.insert(Point(xx, yy));

		x = y = "";
	}
}
*/

void addPoints(std::string text, std::set<Point>& points) {
	std::stringstream stream(text);

	while (1) {
		int x , y;
		char comma;

		stream >> x;
		if (!stream)
			break;

		stream >> comma;
		if (!stream)
			break;

		stream >> y;
		if (!stream)
			break;
		
		points.insert(Point(x, y));
	}
}

Selection* Image::loadSelection(std::string text) {

	//making regex
	std::regex rxName("<name>(.*?)</name>");
	std::smatch nameResult;

	std::regex rxActive("<active>([0-1])</active>");
	std::smatch activeResult;

	std::regex rxPoints("<Points>(.*?)</Points>");
	std::smatch pointsResult;

	std::regex rxPoint("([0-9]+),([0-9]+)");

	//matching begins
	std::regex_search(text, nameResult, rxName);
	std::regex_search(text, activeResult, rxActive);

	//matching all points
	std::regex_search(text, pointsResult, rxPoints);
	std::string path = pointsResult.str(1);

	std::ifstream input(path);

	if (!input)
		throw NonExistentFile();

	std::stringstream buffer;
	buffer << input.rdbuf();
	std::string allPoints = buffer.str();

	//std::string allPoints = pointsResult.str(1);

	std::set<Point> points;
	
	addPoints(allPoints, points);

	std::string name = nameResult.str(1);
	bool active = (bool)atoi(activeResult.str(1).c_str());

	Selection* sel = new Selection(name, points);
	sel->setActive(active);

	return sel;
}

void Image::loadLayers(std::string text) {

	std::regex rxLayers("<Layers>(.*?)</Layers>");		//regex for all layers
	std::smatch layersResult;

	std::regex rxLayer("<Layer>(.*?)</Layer>");			//regex for each layer
	std::smatch layerResult;

	//loading layers
	std::regex_search(text, layersResult, rxLayers);
	std::string allLayers = layersResult.str(1);
	std::sregex_iterator beginL(allLayers.begin(), allLayers.end(), rxLayer);
	std::sregex_iterator endL;

	while (beginL != endL) {
		std::smatch layRes = *beginL++;
		std::string layerInfo = layRes.str(1);
		addLayer(loadLayer(layerInfo));
	}
	//done loading layers
}

void Image::loadSelections(std::string text) {

	std::regex rxSelections("<Selections>(.*?)</Selections>");	//regex for all selections
	std::smatch selectionsResult;

	std::regex rxSelection("<Selection>(.*?)</Selection>");		//regex for each selection
	std::smatch selectionResult;

	//loading selections
	std::regex_search(text, selectionsResult, rxSelections);
	std::string allSelections = selectionsResult.str(1);
	std::sregex_iterator beginS(allSelections.begin(), allSelections.end(), rxSelection);
	std::sregex_iterator endS;

	while (beginS != endS) {
		std::smatch selRes = *beginS++;
		std::string selInfo = selRes.str(1);
		addSelection(loadSelection(selInfo));
	}
	//done loading selections
}

void Image::loadOperations(std::string text) {

	std::regex rxOperations("<Operations>(.*?)</Operations>");
	std::smatch operationsResult;

	//matching
	std::regex_search(text, operationsResult, rxOperations);
	std::string allOperations = operationsResult.str(1);
	
	std::regex rxOperation("<Operation>(.*?)</Operation>");
	std::smatch operationResult;

	std::regex rxSize("<size>(.*?)</size>");	//if size > 0,then it is a composite function

	std::sregex_iterator begin(allOperations.begin(), allOperations.end(), rxOperation);
	std::sregex_iterator end;

	int level = 0;	//number of operations left to load in composite function

	while (begin != end) {
		//moram pomeriti begin
		std::smatch resOp = *begin;
		std::string opInfo = resOp.str(1);
		std::smatch sizeRes;
		std::regex_search(opInfo, sizeRes, rxSize);	//if there is size then it is a composite function

		if (sizeRes.size() > 0) {	//if there is size then it is a composite function		
			addOperation(loadCompositeOperation(begin));
		}
		else {	//it is a basic operation
			addOperation(loadBasicOperation(opInfo));
			begin++;
		}
	}
}


Operation* Image::loadBasicOperation(std::string text) {

	std::regex rxName("<name>(.*?)</name>");
	std::smatch nameResult;

	std::regex rxOpId("<operationId>([0-9]*)</operationId>");
	std::smatch opIdResult;

	std::regex rxArg("<argument>([0-9]*)</argument>");
	std::smatch argResult;

	Operation* op;
	
	//matching
	std::regex_search(text, opIdResult, rxOpId);
	std::regex_search(text, argResult, rxArg);
	std::regex_search(text,nameResult,rxName);
	
	if (opIdResult.size() > 0) {	//if there is size,then it is a basic operation

		OperationsIds id = static_cast<OperationsIds>(atoi(opIdResult.str(1).c_str()));

		if (argResult.size() > 0) {		//if there is size,then operation needs argument
			int x = atoi(argResult.str(1).c_str());
			op = createBasicOperation(id,x);
		}
		else {	//if there is no size,then operation doesn't need argument
			op = createBasicOperation(id);
		}			
	}
	else {	//then it is a predefined function

		auto it = operations.find(nameResult.str(1));

		if (it == operations.end())
			throw NoSuchOperation();

		op = it->second;
	}

	return op;
}

Operation* Image::loadCompositeOperation(std::sregex_iterator& begin) {	//*begin returns information about 1 operation

	std::list<Operation*>* list = new std::list<Operation*>;

	std::sregex_iterator end;

	std::regex rxSize("<size>(.*?)</size>");
	std::smatch sizeResult;

	std::regex rxName("<name>(.*?)</name>");
	std::smatch nameResult;

	std::smatch operationRes = *begin++;
	std::string operationInfo = operationRes.str(1);
	
	std::regex_search(operationInfo, nameResult, rxName);
	std::string name = nameResult.str(1);

	std::regex_search(operationInfo, sizeResult, rxSize);
	int size = atoi(sizeResult.str(1).c_str());
	
	while (begin != end && size > 0) {

		std::smatch opRes = *begin++;
		std::string opInfo = opRes.str(1);

		std::smatch sizeRes;
		std::regex_search(opInfo, sizeRes, rxSize);	//if there is size then it is a composite function

		if (sizeRes.size() > 0) {	//if there is size then it is a composite function		
			list->push_back(loadCompositeOperation(begin));
		}
		else {	//it is a basic operation
			list->push_back(loadBasicOperation(opInfo));
			//begin++;
		}
		size--;
	}


	return new CompositeFunction(name, list);

}

Operation* Image::createBasicOperation(OperationsIds opId) {

	Operation* op = 0;
	int x;

	switch (opId) {
	case ADD:
		std::cout << "Insert adder's value: ";
		std::cin >> x;
		op = new Add(x);
		break;
	case SUB:
		std::cout << "Insert subtractor's value: ";
		std::cin >> x;
		op = new Sub(x);
		break;
	case REV_SUB:
		std::cout << "Insert reverse subtraction's argument: ";
		std::cin >> x;
		op = new RevSub(x);
		break;
	case MUL:
		std::cout << "Insert multiplicator's value: ";
		std::cin >> x;
		op = new Mul(x);
		break;
	case DIV:
		std::cout << "Insert divider's value: ";
		std::cin >> x;
		op = new Div(x);
		break;
	case REV_DIV:
		std::cout << "Insert divisor's value: ";
		std::cin >> x;
		op = new RevDiv(x);
		break;
	case POW:
		std::cout << "Insert exponent ";
		std::cin >> x;
		op = new Pow(x);
		break;
	case LOG:
		op = new Log();
		break;
	case ABS:
		op = new Abs();
		break;
	case MIN:
		std::cout << "Insert minimum's value: ";
		std::cin >> x;
		op = new Min(x);
		break;
	case MAX:
		std::cout << "Insert maximum's value: ";
		std::cin >> x;
		op = new Max(x);
		break;
	default:
		throw NoSuchOperation();
	}

	return op;
}

Operation* Image::createBasicOperation(OperationsIds opId, int x) {

	Operation* op = 0;

	switch (opId) {
	case ADD:
		op = new Add(x);
		break;
	case SUB:
		op = new Sub(x);
		break;
	case REV_SUB:
		op = new RevSub(x);
		break;
	case MUL:
		op = new Mul(x);
		break;
	case DIV:
		op = new Div(x);
		break;
	case REV_DIV:
		op = new RevDiv(x);
		break;
	case POW:
		op = new Pow(x);
		break;
	case MIN:
		op = new Min(x);
		break;
	case MAX:
		op = new Max(x);
		break;
	default:
		throw NoSuchOperation();
	}

	return op;
}

Operation* Image::createCompositeFunction() {

	std::function<void(void)> printOperationsMenu = []() {

		std::cout << "What operation do you want to add?\n";
		std::cout << "0.Addition\n";
		std::cout << "1.Subtraction\n";
		std::cout << "2.Reverse Subtraction\n";
		std::cout << "3.Multiplication\n";
		std::cout << "4.Division\n";
		std::cout << "5.Reverse division\n";
		std::cout << "6.Power\n";
		std::cout << "7.Logarithm\n";
		std::cout << "8.Absolute value\n";
		std::cout << "9.Minimum\n";
		std::cout << "10.Maximum\n";
		std::cout << "11.Another function\n";
		std::cout << "Your choice: ";
	};

	std::string name;
	std::cout << "Insert function's name:\n";
	std::cin >> name;

	int more = 1;

	std::list<Operation*>* ops = new std::list<Operation*>;

	std::cout << "Insert operations:\n";
	//now starts the operations

	while (1) {
		int option;
		Operation* op;
		std::string opName;
		std::cout << "Is there more operations?\n";
		std::cout << "0.No\n1.Yes\n";
		std::cin >> more;

		if (!more)
			break;

		printOperationsMenu();

		std::cin >> option;

		if (option == 11) {
			std::string fname;
			std::cout << "Insert function's name:\n";
			std::cin >> fname;

			auto it = operations.find(fname);

			if (it == operations.end())
				throw NoSuchOperation();

			op = it->second;
		}
		else
			op = createBasicOperation(static_cast<OperationsIds>(option));

		ops->push_back(op);
	}

	return new CompositeFunction(name, ops);
}