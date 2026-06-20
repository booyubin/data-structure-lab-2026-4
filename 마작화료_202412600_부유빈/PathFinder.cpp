#include "PathFinder.h"
#include <algorithm>
#include <sstream>

/**
 * @brief 화료까지의 최단 경로 찾기 (BFS) - 최적화 버전
 */
std::vector<PathStep> PathFinder::findPath(const Hand& startHand, int maxDepth) {
    if (startHand.size() != 14) {
        return {};
    }

    // 이미 화료면 바로 반환
    if (WinChecker::canWin(startHand)) {
        return {};
    }

    // BFS 큐
    std::queue<PathStep> queue;
    std::set<std::string> visited;

    // 시작 상태
    PathStep start;
    start.hand = startHand;
    start.depth = 0;
    queue.push(start);
    visited.insert(handToString(startHand));

    // 경로 추적용 맵
    std::map<std::string, PathStep> parent;

    auto allTiles = getAllTiles();
    
    // 최대 방문 노드 수 제한 (무한 루프 방지)
    const int MAX_VISITED = 10000;
    int visitedCount = 0;

    while (!queue.empty()) {
        PathStep current = queue.front();
        queue.pop();
        
        visitedCount++;
        
        // 너무 많이 탐색하면 중단
        if (visitedCount > MAX_VISITED) {
            return {};
        }

        // 최대 깊이 도달
        if (current.depth >= maxDepth) {
            continue;
        }

        // 현재 손패에서 패 1개씩 버리기
        for (int i = 0; i < current.hand.size(); i++) {
            Tile discardTile = current.hand.getTiles()[i];

            // 모든 가능한 패 가져오기
            for (const auto& drawTile : allTiles) {
                Hand newHand = current.hand.clone();
                newHand.removeTileAt(i);
                newHand.addTile(drawTile);
                newHand.sort();

                std::string handStr = handToString(newHand);

                // 이미 방문한 손패면 스킵
                if (visited.count(handStr)) {
                    continue;
                }

                visited.insert(handStr);

                PathStep nextStep(discardTile, drawTile, newHand, current.depth + 1);

                // 화료 체크
                if (WinChecker::canWin(newHand)) {
                    // 경로 역추적
                    std::vector<PathStep> path;
                    path.push_back(nextStep);

                    std::string currentStr = handToString(current.hand);
                    while (parent.count(currentStr)) {
                        path.push_back(parent[currentStr]);
                        currentStr = handToString(parent[currentStr].hand);
                    }

                    std::reverse(path.begin(), path.end());
                    return path;
                }

                // 큐에 추가
                parent[handStr] = nextStep;
                queue.push(nextStep);
            }
        }
    }

    return {};
}

/**
 * @brief 손패를 고유 문자열로 변환
 */
std::string PathFinder::handToString(const Hand& hand) {
    std::stringstream ss;
    auto tiles = hand.getTiles();
    
    for (const auto& tile : tiles) {
        ss << static_cast<int>(tile.getType()) << tile.getNumber() << ",";
    }
    
    return ss.str();
}

/**
 * @brief 모든 가능한 패 목록
 */
std::vector<Tile> PathFinder::getAllTiles() {
    std::vector<Tile> tiles;

    for (int type = 0; type < 3; type++) {
        for (int num = 1; num <= 9; num++) {
            tiles.push_back(Tile(static_cast<Tile::TileType>(type), num));
        }
    }

    return tiles;
}
