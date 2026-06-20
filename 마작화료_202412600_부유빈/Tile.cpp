#include "Tile.h"

Tile::Tile() : type(MAN), number(1) {}

Tile::Tile(TileType t, int n) : type(t), number(n) {}

bool Tile::operator==(const Tile& other) const {
    return type == other.type && number == other.number;
}

bool Tile::operator!=(const Tile& other) const {
    return !(*this == other);
}

bool Tile::operator<(const Tile& other) const {
    if (type != other.type) {
        return type < other.type;
    }
    return number < other.number;
}

std::string Tile::toString() const {
    std::string result = std::to_string(number);
    
    switch (type) {
        case MAN:
            result += "만";
            break;
        case PIN:
            result += "통";
            break;
        case SOU:
            result += "삭";
            break;
    }
    
    return result;
}
