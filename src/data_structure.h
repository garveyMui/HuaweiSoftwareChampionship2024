#pragma once
#ifndef C_DATA_STRUCTURE_H
#define C_DATA_STRUCTURE_H

//#include <bits/stdc++.h>
/*
 * 只需要引用这个头文件，因为其它类型数据结构头文件由此文件统一引入
 * 添加新类型时注意因此到此头文件
 */
#include "Area.h"
#include "Berth.h"
#include "Boat.h"
#include "Goods.h"
#include "Position.h"
#include "Robot.h"
#include <unordered_set>
// base_data_structure
namespace base_DS{
    const int robot_num = 10;
    const int berth_num = 10;
    const int boat_num = 5;
    const int N = 200;
//    const int n = 200;
//    extern int frame;
    extern int money, boat_capacity, id;       // 当前总钱，每条船容量，帧序号(1开始计数)
    extern int inti_length_to_berth;
    extern bool safe[10];
    extern bool restoring[10];
    extern char ch[N][N];      //地图
    extern int gds[N][N];      //货物坐标
    extern Goods goods[N][N];  //取代gds的数据结构
    extern std::unordered_set<Position, PositionHash> posi_goods;
    extern std::vector<Robot> robot;
    extern std::vector<Berth> berth;
    extern std::vector<Boat> boat;
    extern std::vector<char> symbol_maintained;
    extern std::vector<Position> posi_symbol;

    //连通性数组
    extern int connection[N][N];
}
namespace flag{
    extern bool debug;
};

/*
 * 你不应该使用以下结构体，因为他们已经被扩充且改为class类型
 */


///**
// * 机器人数据结构
// */
//struct Robot
//{
//    int x, y, goods;    // 坐标，是否装货
//    int status;         // 机器人状态
//    int mbx, mby;
//    Robot() {}
//    Robot(int startX, int startY) {
//        x = startX;
//        y = startY;
//    }
//}robot[robot_num + 10];
//
///**
// * 港口数据结构
// */
//struct Berth
//{
//    int x;          //港口坐标
//    int y;
//    int transport_time;     // 一趟运输时间(到虚拟点一趟的时间)
//    int loading_speed;      // 港口装载速度
//    Berth(){}
//    Berth(int x, int y, int transport_time, int loading_speed) {
//        this -> x = x;
//        this -> y = y;
//        this -> transport_time = transport_time;
//        this -> loading_speed = loading_speed;
//    }
//}berth[berth_num + 10];
//
///**
// * 船数据结构
// */
//struct Boat
//{
//    int num, pos, status;       //船容量，船所在港口号，船状态
//}boat[10];




#endif //C_DATA_STRUCTURE_H
