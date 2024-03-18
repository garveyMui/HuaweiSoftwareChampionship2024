//
// Created by Administrator on 2024/3/7.
//
#include <queue>
#include "Goods.h"
#include "Boat.h"
#include "Area.h"
#include <vector>
#include <unordered_map>

using namespace std;

#ifndef LOVEAPPLE_BERTH_H
#define LOVEAPPLE_BERTH_H

class Berth
{
public:
    int id; //文档给的，用处不大

    Position leftupper;     // 港口坐标，注意表示行，左上角为原点，向下为x轴

    Area area;              // 港口所在区域

    int transport_time;     // 一趟运输时间(到虚拟点一趟的时间)

    int loading_speed;      // 港口装载速度，单位为货物个数

    double value;           // loading_speed/transport_time 也许可以变更一下，分母加个常数

    int num_assigned_robots; // 这个港口此时有多少机器人作为目的地

    std::queue<int> queue_goods_value; // 等待上船的港口货物

    std::vector<std::pair<Boat, int>> queue_boats;  //排队进港口的船，计划通过一个for循环选择，后期可做成优先级队列

    int id_boat_in_berth = -1; // 港口内停靠的船舶id

    bool occupied[4][4] = {false}; //表示这个区域是否有机器人 4*4>10 初赛机器人大概率不会排队

    Berth();
    Berth(int x, int y, int transport_time, int loading_speed, int id_berth);

    static bool in_certain_berth(Position posi);

    /*以下0318新加，未实现*/
    vector<vector<APosition>> cells; // 用于存储从港口到其它可达点的路径，可参考get_shortest_path
    int R = 20; // 港口一定半径
    unordered_map<Position, int, PositionHash> territories;  // 一定半径内所有可达点的坐标与对应货品价值，不考虑相交问题
                                                // 相交问题需要全部港口信息，因此不能作为成员变量，
                                                // 相交问题都交给get_init_berth_id, get_revolved_berth_id解决
    int value_ceil; // 对应货品价值超过一定上限之后自愿分给临近港口
    /*以上0318新加，未实现*/
};// Berth



#endif //LOVEAPPLE_BERTH_H
