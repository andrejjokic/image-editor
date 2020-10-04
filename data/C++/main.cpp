#include "Formatter.h"
#include "PAMFormatter.h"
#include "BMPFormatter.h"
#include "Rectangle.h"
#include "Image.h"
#include "Point.h"
#include "Operation.h"
#include <regex>
#include "XMLFormatter.h"
#include <functional>
#include <iostream>
using namespace std;

void loadOperateSave(std::string projectPath,std::string operationPath);
void loadImage(std::string imagePath);
void saveImage(std::string imagePath);

int main(int argc,char* argv[]) {

	if (argc ==  3) {
		loadOperateSave(argv[1],argv[2]);
		exit(1);
	}

	if (argc == 2) {
		loadImage(argv[1]);
		exit(1);
	}

	if (argc == 4) {
		saveImage(argv[1]);
		exit(1);
	}
	
	try {

		//there will be only one instance of image
		Image image;

		//printing all the menu
		function<void(void)> printLoadMenu = []() {

			cout << "Insert option:\n";
			cout << "0. Exit\n";
			cout << "1. New project\n";
			cout << "2. Load project\n";
			cout << "Your choice : ";
		};

		function<void(void)> printOptionMenu = []() {
	
			cout << "Insert option:\n";
			cout << "0. Exit\n";
			cout << "1. Add layer\n";
			cout << "2. Add selection\n";
			cout << "3. Create composite function\n";
			cout << "4. Load composite function\n";
			cout << "5. Set layer's activity:\n";
			cout << "6. Set layer's visibility:\n";
			cout << "7. Set layer's transparency:\n";
			cout << "8. Set selection's activity:\n";
			cout << "9. Do operation\n";
			cout << "10. Remove layer\n";
			cout << "11. Remove Selection\n";
			cout << "12. Save composite function\n";
			cout << "13. Save image\n";
			cout << "14. Save project\n";
			cout << "Your choice : ";
		};

		function<void(void)> printExitMenu = [&image]() {
		
			image.printSaveImageWarning();
			image.printSaveProjectWarning();
			cout << "Insert option:\n";
			cout << "0. Exit\n";
			cout << "1. Save image and exit\n";
			cout << "2. Save project and exit\n";
			cout << "3. Back\n";
			cout << "Your choice : ";
		};

		function<void(void)> printLayerMenu = []() {
		
			cout << "Insert option:\n";
			cout << "0. Add layer with image\n";
			cout << "1. Add empty layer\n";
			cout << "2. Back\n";
			cout << "Your choice : ";
		};

		function<void(void)> printSaveImageMenu = []() {
		
			cout << "Insert option:\n";
			cout << "0. Save as BPM file\n";
			cout << "1. Save as PAM file\n";
			cout << "2. Save as XML file\n";
			cout << "3. Back\n";
			cout << "Your choice : ";
		};

		function<void(void)> loadProject = [&image]() {
		
			cout << "Insert project path:\n";
			std::string path;
			cin >> path;
			image.loadProject(path);
		};

		function<void(void)> saveImage = [&]() {
		
			printSaveImageMenu();
			int option;
			cin >> option;

			if (option == 3)
				return;	//back

			string fileName;

			cout << "Insert file name:\n";
			cin >> fileName;

			switch (option) {

			case 0:
				fileName += ".bmp";
				break;
			case 1:
				fileName += ".pam";
				break;
			case 2:
				fileName += ".xml";
				break;
			}
			
			image.saveImage(fileName);
		};

		function<void(void)> saveProject = [&image]() {
		
			cout << "Insert file name:\n";
			string dest;
			cin >> dest;
			dest += ".xml";
			image.saveProject(dest);
		};

		function<void(void)> exitProject = [&]() {
		
			//image.printSaveWarning();
			printExitMenu();
			int exitOption;
			cin >> exitOption;

			switch (exitOption) {
			case 0:
				cout << "Goodbye!\n";
				exit(1);
			case 1:
				saveImage();
				exit(1);
				break;
			case 2:
				saveProject();
				exit(1);
				break;
			case 3:
				return;
			}
		};

		function<void(void)> addLayer = [&]() {
		
			printLayerMenu();
			int option;
			string path;
			int height, width;
			int noDimension;
			cin >> option;

			switch (option) {
				
			case 0:
				cout << "Insert image path:\n";
				cin >> path;
				image.addLayer(path);
				break;
			case 1:
				cout << "Do you want to specify dimension?:\n";
				cout << "0.No\n";
				cout << "1.Yes\n";
				cin >> noDimension;
				if (!noDimension) {
					image.addLayer();
				}
				else {
					cout << "What is the height?:\n";
					cin >> height;
					cout << "What is the width?:\n";
					cin >> width;
					image.addLayer(height, width);
				}
				break;
			case 2:
				return;
			}
		};

		function<void(void)> addSelection = [&]() {
		
			std::string name;
			std::vector<Rectangle> rects;

			cout << "Insert selection's name:\n";
			cin >> name;

			cout << "Start inserting rectangles information in THIS order:\n";
			cout << "1. Upper left point of a rectangle \t 2.Height \t 3.Width\n";

			while (1) {

				int more;
				cout << "Is there more rectangles?:\n0.No\n1.Yes\n";
				cin >> more;

				if (!more)
					break;
				int x, y, h, w;

				cout << "Insert x coordinate of an upper left point:\n";
				cin >> x;
				cout << "Insert y coordinate of an upper left point:\n";
				cin >> y;
				cout << "Insert height of a rectangle\n";
				cin >> h;
				cout << "Insert width of a rectangle\n";
				cin >> w;

				rects.push_back(Rectangle(Point(x, y), h, w));
			}

			image.addSelection(name, rects);
		};

		function<void(void)> doOperation = [&image]() {
			
			string name;
			cout << "Insert operation's name\n";
			cin >> name;
			image.operate(name);
		};

		function<void(void)> saveCompositeFunction = [&image]() {
		
			cout << "Insert function's name:\n";
			string name;
			cin >> name;
			cout << "Insert destination(with .fun):\n";
			string dest;
			cin >> dest;
			image.saveOperation(name,dest);
		};

		function<void(void)> loadCompositeFunction = [&image]() {
		
			cout << "Insert function's path:\n";
			std::string path;
			cin >> path;

			std::string opName = image.loadCompositeFunction(path);

			cout << "Composite function : " << opName << " has been successfully loaded!\n";
		};

		function<void(void)> setLayerActivity = [&image]() {
		
			int num;
			int val;

			cout << "Insert layer's number:\n";
			cin >> num;
			cout << "Should layer be active?:\n";
			cout << "0.No\n1.Yes\n";
			cin >> val;

			bool should = (val == 1)? true : false;

			image.setLayerActivity(num, should);
		};

		function<void(void)> setLayerVisibility = [&image]() {

			int num;
			int val;

			cout << "Insert layer's number:\n";
			cin >> num;
			cout << "Should layer be visible?:\n";
			cout << "0.No\n1.Yes\n";
			cin >> val;

			bool should = (val == 1) ? true : false;

			image.setLayerVisibility(num, should);
		};

		function<void(void)> setLayerTransparency = [&image]() {

			int num;
			double val;

			cout << "Insert layer's number:\n";
			cin >> num;
			cout << "Insert layer's transparency in %:\n";
			cin >> val;

			image.setLayerTransparency(num, val/100);
		};

		function<void(void)> setSelectionActivity = [&image]() {

			string name;
			int val;

			cout << "Insert selection's name:\n";
			cin >> name;
			cout << "Should selection " + name + " be active?:\n";
			cout << "0.No\n1.Yes\n";
			cin >> val;

			bool should = (val == 1) ? true : false;

			image.setSelectionActivity(name, should);
		};

		function<void(void)> removeLayer = [&image]() {
		
			int num;
			cout << "Insert layer's number:\n";
			cin >> num;
			image.removeLayer(num);
		};

		function<void(void)> removeSelection = [&image]() {

			string name;
			cout << "Insert selection's name:\n";
			cin >> name;
			image.removeSelection(name);
		};

		int loadOption;
		int mainOption;

		printLoadMenu();
		cin >> loadOption;

		switch (loadOption) {	//load Option Switch

		case 0:
			cout << "Goodbye!\n";
			exit(1);
			break;
		case 1:
			break;
		case 2:
			loadProject();
			break;
		}	//end of load option switch


			while (1) {	//OptionMenu
				Operation* op;
				try {

					printOptionMenu();

					cin >> mainOption;

					switch (mainOption) {	//mainOption switch

					case 0:
						exitProject();
						break;
					case 1:
						addLayer();
						break;
					case 2:
						addSelection();
						break;
					case 3:
						op = image.createCompositeFunction();
						image.addOperation(op->getName(), op);
						break;
					case 4:
						loadCompositeFunction();
						break;
					case 5:
						setLayerActivity();
						break;
					case 6:
						setLayerVisibility();
						break;
					case 7:
						setLayerTransparency();
						break;
					case 8:
						setSelectionActivity();
						break;
					case 9:
						doOperation();
						break;
					case 10:
						removeLayer();
						break;
					case 11:
						removeSelection();
						break;
					case 12:
						saveCompositeFunction();
						break;
					case 13:
						saveImage();
						break;
					case 14:
						saveProject();
						break;
					}//end of mainOption switch
					system("CLS");
				}
				catch (std::exception& e) {
					std::cout << e.what() << std::endl;
				}
			}//end of OptionMenu while block


	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void loadOperateSave(std::string projectPath, std::string operationPath) {

	Image image;
	image.loadProject(projectPath);
	std::string opName = image.loadCompositeFunction(operationPath);
	image.operate(opName);
	image.saveProject(projectPath);
}

void loadImage(std::string imagePath) {			//ucita bmp sliku i sacuva je kao xml
	Image image;
	image.addLayer(imagePath);
	imagePath = imagePath.substr(0, imagePath.length() - 3) + "xml";
	image.saveImage(imagePath);
}

void saveImage(std::string imagePath) {
	Image image;
	image.addLayer(imagePath);
	imagePath = imagePath.substr(0, imagePath.length() - 3) + "bmp";		//ucita xml sliku i sacuva je kao bmp
	image.saveImage(imagePath);
}