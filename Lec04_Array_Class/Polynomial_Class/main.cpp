#include <iostream>
#include "Polynomial.h"
using namespace std;

int main() {
    Polynomial a, b, c;

    a.read();   // 다항식 a 입력
    b.read();   // 다항식 b 입력

    c.add(a, b); // c = a + b

    a.display("A = ");
    b.display("B = ");
    c.display("A+B = ");

    return 0;
}