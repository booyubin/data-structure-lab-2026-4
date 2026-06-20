#ifndef TILE_H
#define TILE_H

#include <string>

class Tile {
public:
    enum TileType {
        MAN = 0,
        PIN = 1,
        SOU = 2
    };

private:
    TileType type;
    int number;

public:
    Tile();
    Tile(TileType t, int n);
    
    TileType getType() const { return type; }
    int getNumber() const { return number; }
    
    std::string toString() const;
    
    bool operator==(const Tile& other) const;
    bool operator!=(const Tile& other) const;
    bool operator<(const Tile& other) const;
    
    size_t hash() const;
};

namespace std {
    template<>
    struct hash<Tile> {
        size_t operator()(const Tile& tile) const {
            return tile.hash();
        }
    };
}

#endif
