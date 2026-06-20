#include "Hand.h"
#include <algorithm>
#include <sstream>

/**
 * @brief 기본 생성자
 */
Hand::Hand() {}

/**
 * @brief 패 목록으로 초기화
 */
Hand::Hand(const std::vector<Tile>& t) : tiles(t) {
    sort();
}

/**
 * @brief 패 추가
 */
void Hand::addTile(const Tile& tile) {
    tiles.push_back(tile);
}

/**
 * @brief 패 제거
 */
bool Hand::removeTile(const Tile& tile) {
    auto it = std::find(tiles.begin(), tiles.end(), tile);
    if (it != tiles.end()) {
        tiles.erase(it);
        return true;
    }
    return false;
}

/**
 * @brief 특정 위치의 패 제거
 */
void Hand::removeTileAt(int index) {
    if (index >= 0 && index < (int)tiles.size()) {
        tiles.erase(tiles.begin() + index);
    }
}

/**
 * @brief 손패 정렬
 */
void Hand::sort() {
    std::sort(tiles.begin(), tiles.end());
}

/**
 * @brief 손패 초기화
 */
void Hand::clear() {
    tiles.clear();
}

/**
 * @brief 패 개수 세기
 */
std::map<Tile, int> Hand::countTiles() const {
    std::map<Tile, int> counts;
    for (const auto& tile : tiles) {
        counts[tile]++;
    }
    return counts;
}

/**
 * @brief 특정 패의 개수
 */
int Hand::count(const Tile& tile) const {
    return std::count(tiles.begin(), tiles.end(), tile);
}

/**
 * @brief 손패를 문자열로 변환
 */
std::string Hand::toString() const {
    std::stringstream ss;
    for (size_t i = 0; i < tiles.size(); i++) {
        ss << tiles[i].toString();
        if (i < tiles.size() - 1) {
            ss << " ";
        }
    }
    return ss.str();
}

/**
 * @brief 손패 복사
 */
Hand Hand::clone() const {
    return Hand(tiles);
}
