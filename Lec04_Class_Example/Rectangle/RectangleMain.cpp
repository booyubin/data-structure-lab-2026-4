#include "Rectangle.h"
using namespace std;

int main() {
	// Rectangle 1 °´Ã¼ »ý¼º
	Rectangle rect1(3.4, 2.5);
	cout << "Rectangle 1:" << endl;
	// printf("Rectangle 1:\n");
	cout << "Area: " << rect1.getArea() << endl;
	//
	cout << "Perimeter:" << rect1.getPerimeter() << endl;
	cout << "Is Square ? " << boolalpha << rect1.isSquare() << endl;
	printf("========================================\n");
}