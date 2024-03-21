//
// Created by Administrator on 2024/3/9.
//

#include <cstdio>
#include "update_status.h"
#include "data_structure.h"
#include "choose_a_goods.h"


#include "get_shortest_path.h"
#include <chrono>
#include "utils_debug.h"
#include <fstream>

/*
 * 这个函数负责更新机器人物理状态、远距离目标、近距离目标、到近距离目标的路径
 */
// 先确保不撞墙、撞车恢复之后解决
void update_robot_status (int goods, int x, int y, int sts, Robot& robot) {
    if (robot.dead) {
        return;
    }
    robot.update(goods, x, y, sts);//从环境的状态重新赋值给机器人
    // 假设路径搜索没有问题，那么碰撞都是机器人之间交互导致的
    // 如果没有刚撞车则正常处理，否则交给上层处理
    if (!(robot.last_status==1 && robot.status==0)){
        // 超参数，货稀疏的时候先去港口要紧
//        if (base_DS::id < base_DS::inti_length_to_berth){
//            return;
//        }
        if (base_DS::id < 20){
            return;
        }
        auto getter = ShortestPathGetter();
        // 初始化的远目标都是负责的港口、如果机器人出生在港口则无法处理
        // 没载货就用choose_a_goods

        if (robot.goods == 0) {
            auto start = std::chrono::high_resolution_clock::now(); // 获取当前时间
            std::ofstream file("time.txt", std::ios::out | std::ios::app);
            file << "id " << base_DS::id << endl;
            if (base_DS::goods[robot.current_dest.x][robot.current_dest.y].value <= 0){
                if (robot.posi == robot.destinations.front()->area.center){
                    robot.current_dest = robot.destinations.front()->get_goods_posi();
                    if (robot.current_dest != Position(-1,-1)){
                        robot.path = robot.destinations.front()->berth2point(robot.current_dest);
                    }else{
                        robot.path = {};
                    }
                }else{
                    auto res = getter.shortestPath(robot.posi, robot.sub_dest, 1); // 1:goods
                    robot.current_dest = robot.sub_dest;
                    robot.path = res.second;
                    robot.on_the_way2goods = true;
                }
            }
            // 调用函数
            auto end = std::chrono::high_resolution_clock::now();   // 获取当前时间
//             计算执行时间
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            file << "函数执行时间： " << duration.count()/1000.0 << " ms" << std::endl;
            file.close();
        }else{
            if (robot.last_goods==0){
                // 如果规划路径时发生原地捡现在会出问题
                robot.goods_value = base_DS::goods[robot.last_posi.x][robot.last_posi.y].value;
                base_DS::goods[robot.last_posi.x][robot.last_posi.y].reset();
                base_DS::posi_goods.erase(robot.last_posi);
                for (int i = 0; i < base_DS::berth_num; i++){
                    base_DS::berth[i].reset_goods(robot.last_posi);
                }
                robot.on_the_way2goods = false;
//                auto res = getter.shortestPath(robot.posi, robot.destinations.front().area.center,-1); // 0:berth
//                robot.current_dest = robot.destinations.front().area.center;
//                robot.path = res.second;
//                robot.on_the_way2berth = true; // 系统判定拿到了才置1，这时候不更新远目标，只更新近目标
                robot.current_dest = robot.destinations.front()->area.center;
                robot.destinations.front()->point2berth(robot.posi, robot.path);
                robot.on_the_way2berth = true;
            }else if(robot.path.empty()){ // 被四面围起来撞晕了
//                auto res = getter.shortestPath(robot.posi, robot.destinations.front().area.center,-1); // 0:berth
//                robot.current_dest = robot.destinations.front().area.center;
//                robot.path = res.second;
                robot.current_dest = robot.destinations.front()->area.center;
                robot.destinations.front()->point2berth(robot.posi, robot.path);
                robot.on_the_way2berth = true;
            }
        }
    }

}

void update_boat_status (int sts, int pos, Boat& boat, int boat_id){
    int last_status = boat.status;  //记录船上一次状态
    boat.update(sts, pos);//从交互信息处获取船的状态

    if(boat.status == 0){//处于运输状态
        return;
    }

    // 更新货物状态
    if(boat.status == 1 && boat.id_dest_on_the_way != -1){
        /*自动装货，并更新信息*/
        if (boat.load < boat.capacity && last_status == 1) { //未装满,并且从上一帧船状态就是1了，才能装货
            Berth& berth = base_DS::berth[boat.id_dest_on_the_way];
            berth.id_boat_in_berth = boat.id_boat;
            unsigned long long remove_goods_num = min(berth.loading_speed, boat.capacity - boat.load);
            unsigned long long queue_size = berth.queue_goods_value.size();
            remove_goods_num = min(remove_goods_num, queue_size);

            boat.load += static_cast<int> (remove_goods_num);      //装货
            for (int i = 0; i < remove_goods_num; i++)//移除货物
                berth.queue_goods_value.pop();
        }
    }
    // 更新计划泊位，不用考虑虚拟点，那个在should_go内自动实现
    // 在排队就换但是实际上可以加别的逻辑

//    if ((boat.status == 1 && boat.id_dest_on_the_way == -1)||(boat.status == 2)){
//        if (boat.status == 1 && boat.id_dest_on_the_way == -1)  {
//            boat.load = 0;
//        }    //船到虚拟点,清空载货
//
//       if (base_DS::id > 300){
//           boat.id_dest_in_plan = choose_a_berth(boat);
//       }
//    }
    if (boat.status == 1 && boat.id_dest_on_the_way == -1)  {
        boat.load = 0;
    }    //船到虚拟点,清空载货

//    if (base_DS::id >= 1){
//        boat.id_dest_in_plan = base_DS::scheduler->choose_a_berth(boat);
//    }
}

void update_berth_status(){
    for (int i = 0; i < base_DS::berth_num; i++){
        Berth& berth = base_DS::berth[i];
        // 货物循环
        for (int j = 0; j < base_DS::robot_num; j++){
            Robot& robot = base_DS::robot[j];
            if (robot.just_unload() && berth.area.contains(robot.posi)){
                berth.queue_goods_value.push(robot.goods_value);
                robot.goods_value=0;
            }
        }
        // 船只离港
        int id_boat = berth.id_boat_in_berth;
        if (id_boat!=-1){
            if(base_DS::boat[id_boat].status==0){
                berth.id_boat_in_berth = -1;
            }
        }
    }
}