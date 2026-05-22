#pragma once
#ifndef LOCATION2D_H
#define LOCATION2D_H

struct Location2D {
    int row;   // 현재 위치의 행 번호
    int col;   // 현재 위치의 열 번호

    Location2D(int r = 0, int c = 0) {
        row = r;
        col = c;
    }

    // p가 자신의 이웃인지 검사하는 함수
    bool isNeighbor(Location2D& p) {
        return ((row == p.row && (col == p.col - 1 || col == p.col + 1))
            || (col == p.col && (row == p.row - 1 || row == p.row + 1)));
    }

    // p가 자신과 같은 위치인지 검사
    bool operator==(Location2D& p) {
        return row == p.row && col == p.col;
    }
};

#endif