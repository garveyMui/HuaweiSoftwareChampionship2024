//
// Created by Administrator on 2024/3/7.
//
#include "Position.h"
#ifndef LOVEAPPLE_AREA_H
#define LOVEAPPLE_AREA_H

/*
 * 暂时设置为矩形
 */
class Area {
public:
    int x_lo, x_hi, y_lo, y_hi;  //左闭右开
    Position center;
    Area(int x_lo, int x_hi, int y_lo, int y_hi);
    Area();
    bool contains(Position posi);
    void set_center();
};


#endif //LOVEAPPLE_AREA_H
