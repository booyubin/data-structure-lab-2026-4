#ifndef SPORTSCAR_H
#define SPORTSCAR_H

#include "Car_Class.h"

// 상속 (중요)
class SportsCar : public Car {
public:
    bool bTurbo;

    // 기본 생성자
    SportsCar() {
        bTurbo = false;
    }

    // 터보 설정
    void setTurbo(bool bTur) {
        bTurbo = bTur;
    }

    // 오버라이딩 (핵심)
    void speedUp() override {
        if (bTurbo)
            speed += 20;
        else
            Car::speedUp();
    }
};

#endif
