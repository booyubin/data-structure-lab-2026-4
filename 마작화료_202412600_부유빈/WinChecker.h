#ifndef WINCHECKER_H
#define WINCHECKER_H

#include <vector>
#include <set>
#include "Tile.h"
#include "Hand.h"

/**
 * @class WinChecker
 * @brief 마작 화료(텐파이) 판정 및 대기패 계산
 */
class WinChecker {
public:
    /**
     * @brief 화료 가능 여부 체크 (14장)
     * @param hand 손패
     * @return 화료 가능하면 true
     */
    static bool canWin(const Hand& hand);

    /**
     * @brief 텐파이 여부 체크 (13장)
     * @param hand 손패
     * @return 텐파이면 true
     */
    static bool isTenpai(const Hand& hand);

    /**
     * @brief 대기패 목록 계산 (13장일 때)
     * @param hand 손패
     * @return 대기패 목록
     */
    static std::vector<Tile> getWaitingTiles(const Hand& hand);

    /**
     * @brief 향청패 개수 계산 (14장에서 몇 장만 바꾸면 화료?)
     * @param hand 손패
     * @return 필요한 교환 횟수 (0=화료, 1=1향청, 2=2향청, ...)
     */
    static int getShanten(const Hand& hand);

private:
    /**
     * @brief 재귀적으로 4면자 체크
     * @param counts 패 개수 맵
     * @param mentsus 완성된 면자 개수
     * @return 4면자 완성 가능하면 true
     */
    static bool checkMentsus(std::map<Tile, int> counts, int mentsus);

    /**
     * @brief 순자 제거 시도
     */
    static bool tryRemoveShuntsu(std::map<Tile, int>& counts, const Tile& tile);

    /**
     * @brief 커쯔 제거 시도
     */
    static bool tryRemoveKoutsu(std::map<Tile, int>& counts, const Tile& tile);

    /**
     * @brief 모든 가능한 패 목록 생성
     */
    static std::vector<Tile> getAllPossibleTiles();
};

#endif // WINCHECKER_H
