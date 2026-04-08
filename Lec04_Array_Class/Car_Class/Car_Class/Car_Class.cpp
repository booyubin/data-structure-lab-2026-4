#include "SportsCar.h"

int main() {

    SportsCar car1;

    car1.changeGear(3);
    car1.setTurbo(true);

    car1.speedUp();  // 터보 적용
    car1.display();

    car1.whereAmI();

    return 0;
}
