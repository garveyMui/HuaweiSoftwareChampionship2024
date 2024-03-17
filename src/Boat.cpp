//
// Created by Administrator on 2024/3/7.
//

#include "Boat.h"

Boat::Boat(int capacity_boat, int pos, int status):capacity(capacity_boat), id_dest_on_the_way(pos), status(status) {

}

Boat::Boat() {

}
/*0: 移动中 1: 就绪 2: 等待*/
Boat::Boat(int capacity_boat, int id_boat):capacity(capacity_boat), id_boat(id_boat) {
    this->id_dest_on_the_way = -1;
    this->status = 1;
}

void Boat::update(int status, int id_dest_on_the_way) {
    this->status = status;
    this->id_dest_on_the_way = id_dest_on_the_way;
}
// Boat