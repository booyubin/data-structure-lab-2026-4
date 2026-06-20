#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <queue>
#include <map>
#include <set>
#include "Tile.h"
#include "Hand.h"
#include "WinChecker.h"

/**
 * @struct PathStep
 * @brief 경로의 한 단계 (어떤 패를 버리고 어떤 패를 가져왔는지)
 */
struct PathStep {
    Tile discard;  // 버린 패
    Tile draw;     // 가져온 패
    Hand hand;     // 결과 손패
    int depth;     // 깊이 (교환 횟수)

    PathStep() : depth(0) {}
    PathStep(const Tile& d, const Tile& dr, const Hand& h, int dep)
        : discard(d), draw(dr), hand(h), depth(dep) {}
};

/**
 * @class PathFinder
 * @brief BFS로 화료까지의 최단 경로 탐색
 */
class PathFinder {
public:
    /**
     * @brief 화료까지의 최단 경로 찾기
     * @param startHand 시작 손패
     * @param maxDepth 최대 탐색 깊이 (기본 5)
     * @return 경로 단계 목록 (비어있으면 경로 없음)
     */
    static std::vector<PathStep> findPath(const Hand& startHand, int maxDepth = 5);

    /**
     * @brief 손패를 고유 문자열로 변환 (중복 체크용)
     */
    static std::string handToString(const Hand& hand);

private:
    /**
     * @brief 모든 가능한 패 목록
     */
    static std::vector<Tile> getAllTiles();
};

#endif // PATHFINDER_H
