//
// Created by Administrator on 2024/3/7.
//
#include "Position.h"
#ifndef LOVEAPPLE_GOODS_H
#define LOVEAPPLE_GOODS_H


class Goods {
public:
    int value=0; //价值为0表示这不是货品
    Position posi;
    int remain_frame;
    int lock = -1;   // 只在未被拾取时状态有效：-1表示没锁，其它数字表示id_robot，防止多个机器人同时选中浪费资源，策略中应该每次让步于更近的机器人

    void set_value(int value, Position posi);
    Goods(Position posi, int value, int remain_frame=1000);
    Goods();
    void reset();
    bool decrease_remain();  // 这个函数调用vanish，即返回值为真时应该调用析构函数删除对象
    bool vanish() const;
};


#endif //LOVEAPPLE_GOODS_H
