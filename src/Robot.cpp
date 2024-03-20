//
// Created by 梅佳伟 on 2024/3/7.
//

#include <iostream>
#include <algorithm>

#include "Robot.h"
#include "data_structure.h"
#include "get_shortest_path.h"
#include "utils_debug.h"

Robot::Robot(int startX, int startY, int robot_id=0) {
    this->robot_id = robot_id;
    this->goods_value = 0;
    this->posi = Position(startX, startY);
//    this->last_posi = this->posi;
    this->last_posi = Position();
    this->birth = Position(startX, startY);
    this->_set_area(startX, startY);
    this->goods = 0;  //0：未携带 1：携带
    this->status = 1; //0： 罚时恢复中 1：正常
    this->priority = 0;
    this->penalty = 0; //-1: 表示不要移动
    this->surrounded = 0;
}
Robot::Robot(int startX, int startY, std::vector<Berth> destinations) {
    this->posi = Position(startX, startY);
    this->birth = Position(startX, startY);
    this->_set_area(startX, startY);
    this->goods = 0;  //0：未携带 1：携带
    this->status = 1; //0： 罚时恢复中 1：正常
    this->priority = 0;
    this->penalty = 0; //-1: 表示不要移动
//    this->_set_destinations(destinations);
//    this->current_dest = destinations[0].leftupper;
}
Robot::Robot() {

}
void Robot::_set_area(int startX, int startY) {
    this->area = Area(startX-this->diameter_area,
                      startX+this->diameter_area,
                      startY-this->diameter_area,
                      startY+this->diameter_area);
}

// 判断是否会对撞或者撞停在原地的机器人
bool Robot::will_hit() {
    if (this->path.empty()){
        return false;
    }
    // safe就只做40次循环尽早退出
    bool safe = true;
    for (int i = 0; i < 4; i++){
        Position attempt = Position(this->posi, directions[i]);
        for (int j = 0; j < base_DS::robot_num; j++){
            if (attempt == base_DS::robot[i].posi){
                safe = false;
                break;
            }
        }
    }
    if (safe){
        return false;
    }
    for (int i = 0; i < base_DS::robot_num; i++){
        if (i== this->robot_id){
            continue;
        }
        const Robot & robot_i= base_DS::robot[i];
        // 检测原地
        if (robot_i.path.empty() || robot_i.status==0){
            if (this->path.front()==robot_i.posi){
                return true;
            }
        }else{
            // 检测对撞
            if (this->path.front()==robot_i.posi && robot_i.path.front()==this->posi){
                return true;
            }
        }
    }
    return false;
}

Direction Robot::get_move() {
    // path为空
    if (this->path.empty()){
        return {-1,-1};
    }
    Position attempt = this->path.front();
    int delta_x = attempt.x-this->posi.x;
    int delta_y = attempt.y-this->posi.y;
    return Direction(delta_x, delta_y);
}

Position Robot::reset() {
    return Position(this->birth.x+1, this->birth.y);
//    return this->birth; //暂时总让他回出生地
}

void Robot::_set_destinations() {
    auto getter = ShortestPathGetter();
    Position dest;
    auto res = getter.shortestPath(this->posi, dest, 0);
    if (res.first > 0){
//        for (auto berth:base_DS::berth){
//            if (berth.area.contains(dest)){
//                this->destinations = {berth};
//                break;
//            }
//        }
        for (int i = 0; i < base_DS::berth.size(); i++){
            if (base_DS::berth[i].area.contains(dest)){
                this->destinations = {base_DS::berth[i]};
                break;
            }
        }
        this->path = res.second;
        this->current_dest = dest;
    }else{
        // 说明这个机器人哪个港口都去不了，设置他是死的 以后不更新他的状态
        this->dead = true;
    }
}

void Robot::_set_destinations(std::vector<Berth> berths) {
    this->destinations = berths;
    auto getter = ShortestPathGetter();
    Position dest = berths.front().area.center;
//    auto res = getter.shortestPath(this->posi, dest, -1);
//    if (res.first>0){
//        this->path = res.second;
//        this->current_dest = dest;
//    }else{
//        // 说明这个机器人哪个港口都去不了，设置他是死的 以后不更新他的状态
//        this->dead = true;
//    }
    // 暂时没考虑不连通问题
    this->path = berths.front().point2berth(this->posi);
}

void Robot::_set_init_path() {
    auto getter = ShortestPathGetter();
    Position dest = this->destinations.front().leftupper;
    auto res = getter.shortestPath(this->posi, dest, -1);
    if (res.first > 0){
        this->path = res.second;
        this->current_dest = dest;
    }else{
        // 分配错误
        this->dead = true;
    }
}

void Robot::update(int goods, int x, int y, int status) { //这个函数要写到外面
    this->last_goods = this->goods;
    this->goods = goods;
    this->last_posi = this->posi;
    this->posi = Position(x, y);
    if (!this->path.empty()){
        if (this->posi == this->path.front()){
            this->path.pop();
        }
    }
    this->last_status = this->status;
    this->status = status;
    if(this->status==1){
        this->frame_stop=0;
    }else{
        this->frame_stop++;
    }
    // 这里假设冲突可以被解决
    if (this->last_status == 1 && this->status==0){
        this->penalty = 20;
    }else if(this->last_status == 0 && this->status==0){
        this->penalty--;
    }
    if ((this->status==0)&&(this->frame_stop+this->penalty)!=20){
        this->cannot_be_fixed = true;
    }
}

int Robot::get_move_id() {
    Direction d = this->get_move();
//    std::cout << d.first << d.second;
    if (Robot::move_ids.find(d)!=Robot::move_ids.end()){
        return Robot::move_ids.at(d);
    }else{
        return -1;
    }
}

//状态正常才做装卸货决定
// 暂时不要捡路上意外遇到的
bool Robot::decide_to_load() {

    int x = this->posi.x;
    int y = this->posi.y;
    return base_DS::goods[x][y].value > 0 && this->goods == 0 && this->status == 1 && this->posi == this->current_dest;
}

bool Robot::decide_to_unload() {

    bool in_berth = false;
    for (int i = 0; i < base_DS::berth.size(); i++){
        if (base_DS::berth[i].area.contains(this->posi)){
            in_berth = true;
            break;
        }
    }
//    for (auto berth:base_DS::berth){
//        if (berth.area.contains(this->posi)){
//            in_berth = true;
//            break;
//        }
//    }
    return in_berth && this->goods==1 && this->status==1;
}

bool Robot::just_unload() {
    return this->goods==0 && this->last_goods==1;
}

bool Robot::has_stopped() {
    return this->last_posi==this->posi;
}



/*
 * 有参构造函数
 */