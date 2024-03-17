//
// Created by Administrator on 2024/3/7.
//

#include "Goods.h"
#include "data_structure.h"
/*
 * 第三个值有默认参数，调用时可以赋值
 */
Goods::Goods(Position posi, int value, int remain_frame):posi(posi), value(value), remain_frame(remain_frame){

}
Goods::Goods() {

}
bool Goods::decrease_remain() {
    int current = this->remain_frame;
    this->remain_frame--;
    return this->vanish();
}

bool Goods::vanish() const {
    return this->remain_frame <= 0;
}

void Goods::set_value(int value, Position posi) {
    this->value = value;
    this->remain_frame = 1000;
    this->posi = posi;
    this->lock = -1;
}

void Goods::reset() {
    this->value=0;
    this->remain_frame=0;
    lock=-1;
}

