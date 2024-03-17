//
// Created by 梅佳伟 on 2024/3/12.
//

#include "figure_out.h"
#include "data_structure.h"
#include "get_car_priority.h"
#include "get_shortest_path.h"

#include "utils_debug.h"

// 为unsafe连通集找一个优先级最高的重新规划路线，下一帧启用
void Figure_out::do_figure_out() {
    vector<vector<int>> adjacent_unsafe_robots = connected_robots();

    for (int i = 0; i < adjacent_unsafe_robots.size(); i++){
        int max_robot_id = -1;
        double max_priority = -100.0;
        for (int j = 0; j < adjacent_unsafe_robots[i].size(); j++){
            int robot_id = adjacent_unsafe_robots[i][j];
            Robot robot = base_DS::robot[robot_id];
            double cur_priority = get_car_priority(robot);
            if (cur_priority > max_priority){
                max_robot_id = robot_id;
                max_priority = cur_priority;
            }
        }

        // same direction not figure out
        auto getter = ShortestPathGetter();
        Robot& robot = base_DS::robot[max_robot_id]; //max_robot_id==-1没有处理


        // 目的地是负责港口
        if (Berth::in_certain_berth(robot.current_dest)){
            // 随便找一个港口
            auto res = getter.shortestPath(robot.posi, robot.current_dest,0); // 0:berth
            robot.path = res.second;
        }else{
            bool occupied = false;
            for (int i = 0; i < base_DS::robot_num; i++){
                Robot robot_i = base_DS::robot[i];
                if (!base_DS::safe[i] && (robot_i.posi == robot.current_dest || robot_i.path.front() == robot.current_dest)){
                    occupied = true;
                    break;
                }
            }
            if (!occupied){
                auto res = getter.shortestPath(robot.posi, robot.current_dest,-1); // -1:exact
                robot.path = res.second;
            }else{
                base_DS::goods[robot.current_dest.x][robot.current_dest.y].lock = -1;
                auto res = getter.shortestPath(robot.posi, robot.current_dest,1); // 1:goods
                robot.path = res.second;
                base_DS::goods[robot.current_dest.x][robot.current_dest.y].lock = robot.robot_id;
            }
//            auto res = getter.shortestPath(robot.posi, robot.current_dest,-1); // -1:exact
//            robot.path = res.second;
        }
        // 优先级最高的没被围死
        if (!robot.path.empty()){
            base_DS::safe[max_robot_id] = true;
            robot.surrounded=0;
        }else{
            robot.surrounded++;
        }

    }
}

vector<vector<int>> Figure_out::connected_robots() {
    vector<vector<int>> res;
    vector<int> mark(base_DS::robot_num, -1);
    auto getter = ShortestPathGetter();
    for (int i = 0; i < base_DS::robot_num; i++){
        if (!base_DS::safe[i] && mark[i] == -1){
            getter.get_connected_unsafe_cars(base_DS::robot[i], mark);
        }
    }
    for (int i = 0; i < base_DS::robot_num; i++){
        vector<int>sub_res;
        for (int j = 0; j < base_DS::robot_num; j++){
            if (mark[j]==i){
                sub_res.push_back(j);
            }
        }
        if (!sub_res.empty()){
            res.push_back(sub_res);
        }
    }
    return res;
}


















