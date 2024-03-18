//
// Created by Administrator on 2024/3/7.
//

#include "Berth.h"
#include "data_structure.h"

Berth::Berth(int x, int y, int transport_time, int loading_speed, int id_berth) {
    this->id = id_berth;
    this->leftupper = Position(x, y);
    this->area = Area(x, x+4, y, y+4);
    this->transport_time = transport_time;
    this->loading_speed = loading_speed;
    this->value = (double)loading_speed / transport_time;
    this->num_assigned_robots = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            this->occupied[i][j] = false;
        }
    }

} // Berth
Berth::Berth() {

}

bool Berth::in_certain_berth(Position posi) {
    for (int i = 0; i < base_DS::berth_num; i++){
        if (base_DS::berth[i].area.contains(posi)){
            return true;
        }
    }
    return false;
}
