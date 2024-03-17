//
// Created by Administrator on 2024/3/7.
//

#ifndef LOVEAPPLE_BOAT_H
#define LOVEAPPLE_BOAT_H

class Boat {
public:
    int capacity;  // 文档说这个是船的容量，单位为物品数
    int id_dest_on_the_way;  // 物理泊位、虚拟点则为-1
    int id_dest_in_plan=-1;  // 计划泊位、虚拟点则为-1
    int status;  // 0：运输中、1：装货或运输完成、2：泊位外排队
    int load = 0;//记录装载的货物数量
    int id_boat;
    Boat(int capacity_boat, int pos, int status);
    Boat(int capacity_boat, int id_boat);
    Boat();
    void update(int status, int id_dest_on_the_way);

};// Boat

#endif //LOVEAPPLE_BOAT_H
