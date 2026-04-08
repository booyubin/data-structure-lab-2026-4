#include <iostream>
#include "Polynomial.h"
using namespace std;

// 생성자
Polynomial::Polynomial() {
    degree = 0;
    for (int i = 0; i < MAX_DEGREE; i++)
        coef[i] = 0;
}

// 입력 함수
void Polynomial::read() {
    cout << "다항식 최고 차수 입력: ";
    cin >> degree;

    cout << "계수 입력 (" << degree + 1 << "개): ";
    for (int i = 0; i <= degree; i++) {
        cin >> coef[i];
    }
}

// 출력 함수
void Polynomial::display(const char* str) {
    cout << str;
    for (int i = 0; i < degree; i++) {
        cout << coef[i] << "x^" << (degree - i) << " + ";
    }
    cout << coef[degree] << endl;
}

// 덧셈 함수 (슬라이드 방식 그대로??)
void Polynomial::add(Polynomial a, Polynomial b) {

    if (a.degree > b.degree) {
        *this = a;

        for (int i = 0; i <= b.degree; i++) {
            coef[i + (degree - b.degree)] += b.coef[i];
        }
    }
    else {
        *this = b;

        for (int i = 0; i <= a.degree; i++) {
            coef[i + (degree - a.degree)] += a.coef[i];
        }
    }
}

// 0인지 확인
bool Polynomial::isZero() {
    return degree == 0;
}

// 부호 반전
void Polynomial::negate() {
    for (int i = 0; i <= degree; i++) {
        coef[i] = -coef[i];
    }
}