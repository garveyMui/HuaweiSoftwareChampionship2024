//
// Created by Administrator on 2024/3/14.
//

#include "assign_berth.h"
#include "Berth.h"
#include "data_structure.h"
#include "Robot.h"
#include <vector>
#include <algorithm>

using namespace std;
bool compareBerth(const Berth &a, const Berth &b) {
    return a.value > b.value;
}
// 先不考虑多连通
void assign_berth(Robot & robot){
    vector<Berth>& berths = base_DS::berth;
    std::vector<Berth> berth_choices;
    for(int i = 0; i < base_DS::berth_num; i++){
        if(base_DS::connection[robot.birth.x][robot.birth.y] == base_DS::connection[berths[i].leftupper.x][berths[i].leftupper.y]) {//机器人与码头i联通
            berth_choices.push_back(berths[i]);
        }
    }
    if(berth_choices.empty()){//与任何港口都不连通
        robot.dead = true;
        return;
    }
    // 应该依据连通集个数确定五个港口
    std::sort(berth_choices.begin(), berth_choices.end(), compareBerth);
    for (int i = 0; i < berth_choices.size(); i++){
        if (berth_choices[i].num_assigned_robots<2){
            robot.destinations.push_back(&berths[berth_choices[i].id]);
            berths[berth_choices[i].id].num_assigned_robots++;
            break;
        }
    }
    robot._set_init_path();
}