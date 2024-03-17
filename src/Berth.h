//
// Created by Administrator on 2024/3/7.
//
#include <queue>
#include "Goods.h"
#include "Boat.h"
#include "Area.h"

#ifndef LOVEAPPLE_BERTH_H
#define LOVEAPPLE_BERTH_H

class Berth
{
public:
    int id; //文档给的，用处不大
    Position leftupper;     //港口坐标，注意表示行，左上角为原点，向下为x轴
    Area area;
    int transport_time;     // 一趟运输时间(到虚拟点一趟的时间)
    int loading_speed;      // 港口装载速度，单位为货物个数
    double value;
    int num_assigned_robots;
    Berth();
    Berth(int x, int y, int transport_time, int loading_speed, int id_berth);
    std::queue<int> queue_goods_value;
    std::vector<std::pair<Boat, int>> queue_boats;  //排队进港口的船，计划通过一个for循环选择，后期可做成优先级队列
    bool occupied[4][4] = {false}; //表示这个区域是否与机器人 4*4>10 初赛机器人大概率不会排队
    static bool in_certain_berth(Position posi);
    int id_boat_in_berth = -1;
};// Berth



#endif //LOVEAPPLE_BERTH_H
