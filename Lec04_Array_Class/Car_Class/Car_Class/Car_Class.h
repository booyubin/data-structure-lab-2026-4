#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <cstring>
using namespace std;

class Car {
protected:
    int speed;
    string name;

public:
    int gear;

    // 기본 생성자
    Car() {
        speed = 0;
        gear = 0;
        name = "Noname";
    }

    // 매개변수 생성자
    Car(int s, string n, int g) : speed(s), gear(g) {
        
    }

    // 소멸자
    ~Car() {}

    // 기어 변경
    void changeGear(int g = 4) {
        gear = g;
    }

    // 속도 증가
    virtual void speedUp() {
        speed += 5;
    }

    // 출력
    void display() {
        cout << "[" << name << "] : 기어= " << gear
            << " 속도= " << speed << "km/h " << endl;
    }

    // this 포인터 확인
    void whereAmI() {
        cout << "객체 주소 = " << this << endl;
    }
};

#endif