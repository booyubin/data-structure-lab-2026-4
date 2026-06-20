#include "WinChecker.h"
#include <algorithm>

/**
 * @brief 화료 가능 여부 체크 (14장)
 */
bool WinChecker::canWin(const Hand& hand) {
    if (hand.size() != 14) return false;

    auto counts = hand.countTiles();

    // 모든 패를 머리 후보로 시도
    for (auto& pair : counts) {
        if (pair.second >= 2) {
            // 머리 2개 제거
            std::map<Tile, int> tempCounts = counts;
            tempCounts[pair.first] -= 2;
            if (tempCounts[pair.first] == 0) {
                tempCounts.erase(pair.first);
            }

            // 나머지 12장이 4면자인지 체크
            if (checkMentsus(tempCounts, 0)) {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief 텐파이 여부 체크 (13장)
 */
bool WinChecker::isTenpai(const Hand& hand) {
    if (hand.size() != 13) return false;

    auto waitingTiles = getWaitingTiles(hand);
    return !waitingTiles.empty();
}

/**
 * @brief 대기패 목록 계산
 */
std::vector<Tile> WinChecker::getWaitingTiles(const Hand& hand) {
    if (hand.size() != 13) return {};

    std::vector<Tile> waitingTiles;
    auto allTiles = getAllPossibleTiles();

    for (const auto& tile : allTiles) {
        Hand testHand = hand.clone();
        testHand.addTile(tile);

        if (canWin(testHand)) {
            waitingTiles.push_back(tile);
        }
    }

    return waitingTiles;
}

/**
 * @brief 향청패 개수 계산
 */
int WinChecker::getShanten(const Hand& hand) {
    if (hand.size() != 14 && hand.size() != 13) return -1;

    // 0향청: 화료
    if (hand.size() == 14 && canWin(hand)) {
        return 0;
    }

    // 1향청: 텐파이 (13장)
    if (hand.size() == 13 && isTenpai(hand)) {
        return 1;
    }

    // 1향청: 14장에서 1장 버리면 텐파이
    if (hand.size() == 14) {
        for (int i = 0; i < hand.size(); i++) {
            Hand testHand = hand.clone();
            testHand.removeTileAt(i);
            
            if (isTenpai(testHand)) {
                return 1;
            }
        }
    }

    // 2향청: 1장 교환으로 텐파이
    auto allTiles = getAllPossibleTiles();
    
    if (hand.size() == 14) {
        for (int i = 0; i < hand.size(); i++) {
            for (const auto& tile : allTiles) {
                Hand testHand = hand.clone();
                testHand.removeTileAt(i);
                testHand.addTile(tile);
                
                if (isTenpai(testHand)) {
                    return 2;
                }
            }
        }
    }

    // 3향청 이상
    return 3;
}


/**
 * @brief 재귀적으로 4면자 체크
 */
bool WinChecker::checkMentsus(std::map<Tile, int> counts, int mentsus) {
    // 4면자 완성
    if (mentsus == 4) {
        return counts.empty();
    }

    // 남은 패가 없으면 실패
    if (counts.empty()) {
        return false;
    }

    // 첫 번째 패 선택
    Tile firstTile = counts.begin()->first;

    // 커쯔 시도 (같은 패 3개)
    if (counts[firstTile] >= 3) {
        std::map<Tile, int> tempCounts = counts;
        if (tryRemoveKoutsu(tempCounts, firstTile)) {
            if (checkMentsus(tempCounts, mentsus + 1)) {
                return true;
            }
        }
    }

    // 순자 시도 (연속된 3개)
    if (tryRemoveShuntsu(counts, firstTile)) {
        if (checkMentsus(counts, mentsus + 1)) {
            return true;
        }
    }

    return false;
}

/**
 * @brief 순자 제거 시도
 */
bool WinChecker::tryRemoveShuntsu(std::map<Tile, int>& counts, const Tile& tile) {
    // 9는 순자 시작 불가
    if (tile.getNumber() >= 8) return false;

    Tile tile2(tile.getType(), tile.getNumber() + 1);
    Tile tile3(tile.getType(), tile.getNumber() + 2);

    if (counts[tile] >= 1 && counts[tile2] >= 1 && counts[tile3] >= 1) {
        counts[tile]--;
        counts[tile2]--;
        counts[tile3]--;

        if (counts[tile] == 0) counts.erase(tile);
        if (counts[tile2] == 0) counts.erase(tile2);
        if (counts[tile3] == 0) counts.erase(tile3);

        return true;
    }

    return false;
}

/**
 * @brief 커쯔 제거 시도
 */
bool WinChecker::tryRemoveKoutsu(std::map<Tile, int>& counts, const Tile& tile) {
    if (counts[tile] >= 3) {
        counts[tile] -= 3;
        if (counts[tile] == 0) {
            counts.erase(tile);
        }
        return true;
    }
    return false;
}

/**
 * @brief 모든 가능한 패 목록 생성
 */
std::vector<Tile> WinChecker::getAllPossibleTiles() {
    std::vector<Tile> tiles;

    for (int type = 0; type < 3; type++) {
        for (int num = 1; num <= 9; num++) {
            tiles.push_back(Tile(static_cast<Tile::TileType>(type), num));
        }
    }

    return tiles;
}
