//
// Created by Administrator on 2024/3/7.
//

#include "Area.h"
Area::Area(int x_lo, int x_hi,
           int y_lo, int y_hi):x_lo(x_lo), x_hi(x_hi), y_lo(y_lo), y_hi(y_hi){
            set_center();
}
Area::Area() {
    this->x_lo = 0;
    this->x_hi = 200;  // 左闭右开
    this->y_lo = 0;
    this->y_hi = 200;
    set_center();
}

bool Area::contains(Position posi) {
    bool contained = (this->x_lo<=posi.x &&
            posi.x<this->x_hi && this->y_lo<=posi.y && posi.y<this->y_hi);
    return contained;
}
void Area::set_center(){
    center.x = (x_lo + x_hi) / 2;
    center.y = (y_lo + y_hi) / 2;
}
