#include <stdio.h>
#include <stack>
#include <deque>
#include "Location2D.h"

using namespace std;

const int MAZE_SIZE = 6;

char map[MAZE_SIZE][MAZE_SIZE] = {
    { '1', '1', '1', '1', '1', '1' },
    { 'e', '0', '1', '0', '0', '1' },
    { '1', '0', '0', '0', '1', '1' },
    { '1', '0', '1', '0', '1', '1' },
    { '1', '0', '1', '0', '0', 'x' },
    { '1', '1', '1', '1', '1', '1' },
};

// 위치가 유효한지 검사 (0이거나 x인 경우만 이동 가능)
bool isValidLoc(int r, int c)
{
    if (r < 0 || c < 0 || r >= MAZE_SIZE || c >= MAZE_SIZE)
        return false;
    else
        return map[r][c] == '0' || map[r][c] == 'x';
}

// 미로를 원래 상태로 초기화
void resetMap()
{
    char temp[MAZE_SIZE][MAZE_SIZE] = {
        { '1', '1', '1', '1', '1', '1' },
        { 'e', '0', '1', '0', '0', '1' },
        { '1', '0', '0', '0', '1', '1' },
        { '1', '0', '1', '0', '1', '1' },
        { '1', '0', '1', '0', '0', 'x' },
        { '1', '1', '1', '1', '1', '1' },
    };

    for (int i = 0; i < MAZE_SIZE; i++)
        for (int j = 0; j < MAZE_SIZE; j++)
            map[i][j] = temp[i][j];
}

// -----------------------------------------------------------------
// [원래 방식] 팝(Pop)할 때 방문 처리 함수들
// -----------------------------------------------------------------

void DFS_stack_origin()
{
    stack<Location2D> locStack;
    Location2D entry(1, 0);
    locStack.push(entry);

    while (locStack.empty() == false) {
        Location2D here = locStack.top();
        locStack.pop();

        int r = here.row;
        int c = here.col;
        printf("(%d,%d) ", r, c);

        if (map[r][c] == 'x') {
            printf("\nDFS 미로 탐색 성공\n");
            return;
        }
        else {
            map[r][c] = '.'; // Pop할 때 방문 처리 (중복 push 발생 위험)

            if (isValidLoc(r - 1, c)) locStack.push(Location2D(r - 1, c));
            if (isValidLoc(r + 1, c)) locStack.push(Location2D(r + 1, c));
            if (isValidLoc(r, c - 1)) locStack.push(Location2D(r, c - 1));
            if (isValidLoc(r, c + 1)) locStack.push(Location2D(r, c + 1));
        }
    }
    printf("DFS 미로 탐색 실패\n");
}

void BFS_deque_origin()
{
    deque<Location2D> locDeque;
    Location2D entry(1, 0);
    locDeque.push_back(entry);

    while (locDeque.empty() == false) {
        Location2D here = locDeque.front();
        locDeque.pop_front();

        int r = here.row;
        int c = here.col;
        printf("(%d,%d) ", r, c);

        if (map[r][c] == 'x') {
            printf("\nBFS 미로 탐색 성공\n");
            return;
        }
        else {
            map[r][c] = '.'; // Pop할 때 방문 처리 (중복 push 발생 위험)

            if (isValidLoc(r - 1, c)) locDeque.push_back(Location2D(r - 1, c));
            if (isValidLoc(r + 1, c)) locDeque.push_back(Location2D(r + 1, c));
            if (isValidLoc(r, c - 1)) locDeque.push_back(Location2D(r, c - 1));
            if (isValidLoc(r, c + 1)) locDeque.push_back(Location2D(r, c + 1));
        }
    }
    printf("BFS 미로 탐색 실패\n");
}


// -----------------------------------------------------------------
// [개선된 방식] 푸시(Push)할 때 즉시 방문 처리 함수들
// -----------------------------------------------------------------

void DFS_stack_improved()
{
    stack<Location2D> locStack;
    Location2D entry(1, 0);
    locStack.push(entry);
    map[entry.row][entry.col] = '.'; // 시작점 방문 처리

    while (locStack.empty() == false) {
        Location2D here = locStack.top();
        locStack.pop();

        int r = here.row;
        int c = here.col;
        printf("(%d,%d) ", r, c);

        // 상하좌우를 보며 갈 수 있는 곳이 있다면 Push할 때 즉시 '.' 처리
        // 단, 목적지 'x'를 바로 덮어쓰지 않도록 유의
        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (isValidLoc(nr, nc)) {
                if (map[nr][nc] == 'x') {
                    printf("(%d,%d) \nDFS 미로 탐색 성공\n", nr, nc);
                    return;
                }
                map[nr][nc] = '.'; // Push할 때 즉시 방문 처리하여 중복 방지
                locStack.push(Location2D(nr, nc));
            }
        }
    }
    printf("DFS 미로 탐색 실패\n");
}

void BFS_deque_improved()
{
    deque<Location2D> locDeque;
    Location2D entry(1, 0);
    locDeque.push_back(entry);
    map[entry.row][entry.col] = '.'; // 시작점 방문 처리

    while (locDeque.empty() == false) {
        Location2D here = locDeque.front();
        locDeque.pop_front();

        int r = here.row;
        int c = here.col;
        printf("(%d,%d) ", r, c);

        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (isValidLoc(nr, nc)) {
                if (map[nr][nc] == 'x') {
                    printf("(%d,%d) \nBFS 미로 탐색 성공\n", nr, nc);
                    return;
                }
                map[nr][nc] = '.'; // Push할 때 즉시 방문 처리하여 중복 방지
                locDeque.push_back(Location2D(nr, nc));
            }
        }
    }
    printf("BFS 미로 탐색 실패\n");
}

// -----------------------------------------------------------------
// 메인 함수
// -----------------------------------------------------------------
int main()
{
    int menu_num;

    while (true) {
        printf("\n============================================\n");
        printf(" 1) [원래 방식] Stack 이용 DFS\n");
        printf(" 2) [원래 방식] Deque 이용 BFS\n");
        printf(" 3) [개선 방식] Stack 이용 DFS (즉시 방문처리)\n");
        printf(" 4) [개선 방식] Deque 이용 BFS (즉시 방문처리)\n");
        printf(" 5) 프로그램 종료\n");
        printf("============================================\n");
        printf("메뉴를 선택하세요: ");

        // scanf_s 보안 경고 대응용 (환경에 따라 scanf로 변경 가능)
        if (scanf_s("%d", &menu_num) != 1) break;

        switch (menu_num) {
        case 1:
            printf("\n[원래 방식] Stack DFS 탐색 경로:\n");
            resetMap();
            DFS_stack_origin();
            break;

        case 2:
            printf("\n[원래 방식] Deque BFS 탐색 경로:\n");
            resetMap();
            BFS_deque_origin();
            break;

        case 3:
            printf("\n[개선 방식] 즉시 방문처리 Stack DFS 탐색 경로:\n");
            resetMap();
            DFS_stack_improved();
            break;

        case 4:
            printf("\n[개선 방식] 즉시 방문처리 Deque BFS 탐색 경로:\n");
            resetMap();
            BFS_deque_improved();
            break;

        case 5:
            printf("프로그램을 종료합니다.\n");
            return 0;

        default:
            printf("잘못된 입력입니다. 1~5 사이의 숫자를 입력해주세요.\n");
            break;
        }
    }
    return 0;
}