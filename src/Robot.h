//
// Created by Administrator on 2024/3/7.
//
#include "Area.h"
#include "Berth.h"
#include <vector>
#include <queue>
#include <map>
#include <unordered_set>
//#include <bits/stdc++.h>

#ifndef LOVEAPPLE_ROBOT_H
#define LOVEAPPLE_ROBOT_H
/**
 * 机器人数据结构
 */
using Direction = std::pair<int, int>;

class Robot {
public:
    static std::map<Direction, int> move_ids;

    Robot();

    Robot(int startX, int startY, int robot_id);

    Robot(int startX, int startY, std::vector<Berth> destinations);
    int surrounded;
    int robot_id;
    Position posi;
    Position last_posi;
    Position birth;
    int goods;    // 是否装货
    int last_goods=0;    // 用于确认货品被捡起或者放下
    // Goods gd; //装载的货物
    int goods_value;
    std::unordered_set<Position, PositionHash> posi_goods_in_view; //视野内的货物
    std::unordered_set<Position, PositionHash> posi_goods_in_area; //负责区域内的货物
    int status;         // 机器人状态
    int last_status = 1; // 用于判断装车恢复
    bool dead = false;
    bool on_the_way2berth = false;
    bool on_the_way2goods = false;
    bool cannot_be_fixed = false; // 用来调试装车调度逻辑
    Position current_dest;       /* 计划前往的位置，mb是目标，
                         * 这里我们设置为一个较远的位置而不是下一个位置
                         */
    Position sub_dest;  // 用于A*的子目标
    int diameter_area = 20;
    double priority;    // 错车优先级
    Area area;          // 所属区域，用来确定关心的货物
    std::vector<Berth> destinations;  // 负责的目的地港口
    int penalty;         // 碰撞剩余罚时，单位为帧 默认所有时间单位都为帧
    int frame_stop;      // 碰撞导致的此次停车帧数计数
    int view = 30;         // 视野半径，只计算这以内的货物距离与路径
    std::queue<Position> path;  //用于前进的路径栈，因为get_shortest_path返回值是栈
    std::vector<Direction> directions = {{1,  0},
                                         {-1, 0},
                                         {0,  1},
                                         {0,  -1}};

    void update(int goods, int x, int y, int status);
    void _set_area(int startX, int startY);
    void _set_destinations();
    void _set_destinations(std::vector<Berth> berth);
    void _set_init_path();
    bool will_hit();  // 计划前往的位置是否会与其它机器人相撞
    Position reset();     // 用于为在港口装卸完的机器人从Area中选一个闲逛目标
    bool just_unload();
    bool has_stopped();
    Direction get_move();       //从path中取出下一个位置并计算前进方向
    int get_move_id();
    bool decide_to_load();
    bool decide_to_unload();

};// Robot

#endif
//LOVEAPPLE_ROBOT_H
