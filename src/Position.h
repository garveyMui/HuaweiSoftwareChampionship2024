//
// Created by Administrator on 2024/3/7.
//

#ifndef LOVEAPPLE_POSITION_H
#define LOVEAPPLE_POSITION_H


#include <utility>
#include <cstddef>
#include <iostream>

class Position {
public:
    int x, y;
    Position(int x, int y);
    Position();
    Position(Position posi, std::pair<int,int> direction);
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    bool operator<(const Position& other) const;
    int operator-(const Position& other) const; // 曼哈顿距离
    friend std::ostream & operator<<(std::ostream &out, Position& p);
    char get_symbol();
};

class APosition:public Position {
public:

    int f, g, h; //for a*
    APosition* parent; //for a*
    int level = 0;

    APosition(int row, int col, int g, int h);
    APosition();
};

//自定义哈希函数
struct PositionHash{
    size_t operator()(const Position& p) const;
};


#endif //LOVEAPPLE_POSITION_H
