//
// Created by Administrator on 2024/3/7.
//

#include "Position.h"
#include "data_structure.h"
#include <iostream>
#include <cmath>
#include <climits>
APosition::APosition(int row, int col, int g, int h) : Position(row, col), g(g), h(h) {

    this->f = g + h;
}

APosition::APosition() {
    this->x = -1;
    this->y = -1;
    this->f = 0;
    this->g = 0;
    this->h = 0;
    this->parent = nullptr;
}

Position::Position(int x, int y):x(x), y(y) {

};
Position::Position(){
    this->x = -1;
    this->y = -1;
}
Position::Position(Position posi, std::pair<int, int> direction) {
    this->x = posi.x+direction.first;
    this->y = posi.y+direction.second;
}
bool Position::operator==(const Position& other) const {
    return (this->x == other.x) && (this->y == other.y);
}
bool Position::operator!=(const Position& other) const {
    return !((this->x == other.x) && (this->y == other.y));
}
int Position::operator-(const Position& other) const {
    int delta_x = abs(this->x-other.x);
    int delta_y = abs(this->y-other.y);
    return delta_x+delta_y;
}
bool Position::operator<(const Position& other) const {
    if (this->x < other.x){
        return true;
    }else{
        return this->y == other.y;
    }
}

std::ostream & operator<<(std::ostream &out, Position& p){
    out << p.x << ", " << p.y;
    return out;
}

char Position::get_symbol() {
    return base_DS::ch[this->x][this->y];
}

size_t PositionHash::operator()(Position &p) const {
    return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
}
