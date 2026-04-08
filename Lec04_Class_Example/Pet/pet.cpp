// pet.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "Pet.h"
using namespace std;
int main() {
	Pet myPet("Kong", 5, "dog");
	cout << "My pet name is " << myPet.getName() << endl;
	//puppy class test
	Puppy myPuppy("Lucky", 10, "dog", "Puddle");
	cout << "My puppy's breed is " << myPuppy.getBreed() << endl;
}