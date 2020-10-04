#pragma once
#include "Pixel.h"
#include "Formatter.h"
#include <list>
#include <vector>

//enum OperationsIds { ADD, SUB, REV_SUB, MUL, DIV, REV_DIV, POW, LOG, ABS, MIN, MAX, INVERSE, GRAY, BLACKWHITE, MEDIAN, COMPOSITE };

class CompositeFunction;
class Point;

//operation name is: Class name + Argument [example : Add5]

class Operation {
public:
	Operation(std::string name) : name(name) {}
	void operator()(Pixel& pix);		//this can be called even when operation is not atomic
	void operator()(ImageInfo& image);	//this must be called if operation is atomic(Median...)
	std::string getName()const { return name; }
protected:
	friend class CompositeFunction;
	friend class XMLFormatter;
	virtual void operate(Pixel& pix) = 0;
	virtual void operate(ImageInfo& image) = 0;
	virtual std::string writeXML() = 0;
	
	std::string name;
private:
	void correctPixel(Pixel& pixel)const;
};

class Add : public Operation {
public:
	//Add(int x) : Operation("Add " + std::to_string(x)) , x(x) {};
	Add(int x) : Operation("Addition"), x(x) {};
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class Sub : public Operation {
public:
	//Sub(int x) : Operation("Sub " + std::to_string(x)) , x(x) {};
	Sub(int x) : Operation("Subtraction"), x(x) {};
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class RevSub : public Operation {
public:
	//RevSub(int x) : Operation("RevSub " + std::to_string(x)) , x(x) {};
	RevSub(int x) : Operation("Reverse Subtraction"), x(x) {};
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class Mul : public Operation {
public:
	//Mul(int x) : Operation("Mul " + std::to_string(x)) , x(x) {};
	Mul(int x) : Operation("Multiplication"), x(x) {};
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class Div : public Operation {
public:
	Div(int x);
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class RevDiv : public Operation {
public:
	//RevDiv(int x) : Operation("RevDiv " + std::to_string(x)), x(x) {}
	RevDiv(int x) : Operation("Reverse Division"), x(x) {}
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class Pow : public Operation {
public:
	//Pow(int x) : Operation("Pow " + std::to_string(x)) , x(x) {}
	Pow(int x) : Operation("Power"), x(x) {}
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class Log : public Operation {
public:
	//Log() : Operation("Log") {}
	Log() : Operation("Logarithm") {}
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
};

class Abs : public Operation {
public:
	//Abs() : Operation("Abs") {}
	Abs() : Operation("Absolute value") {}
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
};

class Min : public Operation {
public:
	//Min(int x) : Operation("Min " + std::to_string(x)) , x(x) {}
	Min(int x) : Operation("Minimum"), x(x) {}
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class Max : public Operation {
public:
	//Max(int x) : Operation("Max " + std::to_string(x)) , x(x) {}
	Max(int x) : Operation("Maximum"), x(x) {}
protected:
	void operate(Pixel& pixel)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	int x;
};

class Inverse : public Operation {
public:
	Inverse() : Operation("Inverse") {}
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
};

class Gray : public Operation {
public:
	Gray() : Operation("Gray") {}
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
};

class BlackWhite : public Operation {
public:
	BlackWhite() : Operation("BlackWhite") {}
protected:
	void operate(Pixel& pix)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
};

class CompositeFunction : public Operation {
public:
	CompositeFunction(std::string name, std::list<Operation*>* operations);
	~CompositeFunction();
protected:
	virtual void operate(Pixel& pixel)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
private:
	std::list<Operation*>* operations;
};


class Median : public Operation {
public:
	Median() : Operation("Median") {}
protected:
	void operate(Pixel& pixel)override;
	void operate(ImageInfo& image)override;
	std::string writeXML()override;
};
