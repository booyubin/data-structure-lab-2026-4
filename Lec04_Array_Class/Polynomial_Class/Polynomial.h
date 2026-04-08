#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#define MAX_DEGREE 80

class Polynomial {
private:
    int degree;               // 최고 차수
    float coef[MAX_DEGREE];   // 계수 배열

public:
    Polynomial();             // 생성자

    void read();              // 입력
    void display(const char* str = "Poly = "); // 출력
    void add(Polynomial a, Polynomial b); // 덧셈

    bool isZero();            // 0인지 확인
    void negate();            // 부호 반전
};

#endif
