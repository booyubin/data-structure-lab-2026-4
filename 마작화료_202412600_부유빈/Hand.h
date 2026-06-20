#ifndef HAND_H
#define HAND_H

#include <vector>
#include <map>
#include "Tile.h"

/**
 * @class Hand
 * @brief 플레이어의 손패(14장)를 관리하는 클래스
 */
class Hand {
private:
    std::vector<Tile> tiles;  // 손패 목록

public:
    /**
     * @brief 기본 생성자
     */
    Hand();

    /**
     * @brief 패 목록으로 초기화
     */
    Hand(const std::vector<Tile>& t);

    /**
     * @brief 손패 개수 반환
     */
    int size() const { return tiles.size(); }

    /**
     * @brief 손패 목록 반환
     */
    const std::vector<Tile>& getTiles() const { return tiles; }

    /**
     * @brief 패 추가
     */
    void addTile(const Tile& tile);

    /**
     * @brief 패 제거
     * @return 성공 여부
     */
    bool removeTile(const Tile& tile);

    /**
     * @brief 특정 위치의 패 제거
     */
    void removeTileAt(int index);

    /**
     * @brief 손패 정렬 (타입별, 숫자별)
     */
    void sort();

    /**
     * @brief 손패 초기화
     */
    void clear();

    /**
     * @brief 패 개수 세기 (맵 형태로 반환)
     * @return {Tile -> 개수}
     */
    std::map<Tile, int> countTiles() const;

    /**
     * @brief 특정 패의 개수
     */
    int count(const Tile& tile) const;

    /**
     * @brief 손패를 문자열로 변환
     * @return 예: "1m 2m 3m 4p 5p 6p 7s 8s 9s 1m 1m 2p 2p 2p"
     */
    std::string toString() const;

    /**
     * @brief 손패 복사
     */
    Hand clone() const;
};

#endif // HAND_H
