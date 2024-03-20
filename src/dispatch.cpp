//
// Created by 梅佳伟 on 2024/3/12.
//

#include "dispatch.h"
#include "data_structure.h"
#include "utils_debug.h"

using namespace std;
void Dispatch::do_dispatch() {
//    restore();
//    auto start = std::chrono::high_resolution_clock::now(); // 获取当前时间
    modify_map();
    // 调用函数
//    auto end = std::chrono::high_resolution_clock::now();   // 获取当前时间
//             计算执行时间
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//    std::cout << "2函数执行时间： " << duration.count()/1000.0 << " ms" << std::endl;

    modify_safe();
}

// 交换位置或对方正常但不想动
bool Dispatch::will_collide_eachOther(const Robot& a, const Robot& b) {
    if (a.path.empty() && b.path.empty()){
        return false;
    }
    if (a.path.empty()){
        if (b.path.front()==a.posi){
            return true;
        }
    }
    if (b.path.empty()){
        if (a.path.front()==b.posi){
            return true;
        }
    }
    return (a.path.front()==b.posi) && (a.posi==b.path.front());
}

// 想移动到同一位置也添加到这里
bool Dispatch::wanna_exchange(Robot &a, Robot &b) {

    // 对方unsafe暂停导致的碰撞
    // 之前的逻辑同目标停止第二帧会错误进入对方停车分支
    if(a.has_stopped()){
        if (b.path.front()==a.posi){
            return true;
        }
    }
    if(b.has_stopped()){
        if (a.path.front()==b.posi){
            return true;
        }
    }
    bool exchange = (a.path.front()==b.posi) && (a.posi==b.path.front());
    bool same_next = a.path.front()==b.path.front();
    return exchange || same_next;
}

Dispatch::Dispatch(int num_robot) {

}

void Dispatch::restore() {
    for (int i = 0; i < base_DS::posi_symbol.size(); i++){
        Position posi = base_DS::posi_symbol[i];
        base_DS::ch[posi.x][posi.y] = base_DS::symbol_maintained[i];

    }

    base_DS::posi_symbol.clear();
    base_DS::symbol_maintained.clear();
    // safe全部重置，以免将A-><-A误判为unsafe set
    for (int i = 0; i < base_DS::robot_num; i++){
        base_DS::safe[i] = true;
    }
}

void Dispatch::modify_map() {
    for (int i = 0; i < base_DS::robot_num; i++){
        Robot& robot_i = base_DS::robot[i];

        char& sym = base_DS::ch[robot_i.posi.x][robot_i.posi.y];
//        if (((robot_i.status==0) || robot_i.path.empty()) && sym != wall){
//            // ||(robot_i.has_stopped())
//            base_DS::symbol_maintained.push_back(sym);
//            sym = wall;
//            base_DS::posi_symbol.push_back(robot_i.posi);
//        }
        // 修改为机器人的位置都不可通行
        if (sym != wall){
            base_DS::symbol_maintained.push_back(sym);
            sym = wall;
            base_DS::posi_symbol.push_back(robot_i.posi);

        }
    }
    for (int i = 0; i < base_DS::robot_num - 1; i++){
        Robot& robot_i = base_DS::robot[i];
        if (robot_i.status==0 || robot_i.path.empty()){
            continue;
        }
        for (int j = i+1; j < base_DS::robot_num; j++) {
            Robot& robot_j = base_DS::robot[j];
            if (robot_j.status==0 || robot_j.path.empty()){
                continue;
            }
//            robot_j.path = robot_i.path; //调试撞击检测用
            bool will_collide = wanna_exchange(robot_i, robot_j);
            if (will_collide){
//                char &sym0 = base_DS::ch[robot_i.posi.x][robot_i.posi.y];
//                if (sym0 != '#') {
//                    base_DS::symbol_maintained.push_back(sym0);
//                    sym0 = wall;
//                    base_DS::posi_symbol.push_back(robot_i.posi);
//                }
//                char &sym1 = base_DS::ch[robot_j.posi.x][robot_j.posi.y];
//                if (sym1 != '#') {
//                    base_DS::symbol_maintained.push_back(sym1);
//                    sym1 = wall;
//                    base_DS::posi_symbol.push_back(robot_j.posi);
//                }
                if (robot_i.path.front()==robot_j.path.front()){
                    Position attempt = robot_j.path.front();
                    char &sym2 = base_DS::ch[attempt.x][attempt.y];
                    if (sym2 != '#') {
                        base_DS::symbol_maintained.push_back(sym2);
                        sym2 = wall;
                        base_DS::posi_symbol.push_back(attempt);
                    }
                }

            }
        }
    }
}

void Dispatch::modify_safe() {
    for (int i = 0; i < base_DS::robot_num; i++){
        Robot robot = base_DS::robot[i];
        if (base_DS::robot[i].path.empty()){
            continue;
        }
        if (base_DS::robot[i].path.front().get_symbol() == wall){
            base_DS::safe[i]=false;
            Position current_dest = base_DS::robot[i].current_dest;
            base_DS::goods[current_dest.x][current_dest.y].lock = -1; // 去锁
        }
        // 这版考虑更通用的避障策略 但是显著增加寻路次数
//        // 这必须放在最后，不然开局走不动 还没想为啥是这样
//        if (base_DS::robot[i].posi.get_symbol() == wall){
//            base_DS::safe[i]=false;
//        }
    }
}
