//
// Created by Administrator on 2024/3/18.
//

#include "get_revolved_berth_id.h"

#include "get_shortest_path.h"
#include "data_structure.h"

#include <iostream>
#include <algorithm>
#include <numeric>

get_revolved_berth_id::get_revolved_berth_id() = default;


bool get_revolved_berth_id::isGoods(Position posi, int length) {
    const Goods& tmp = base_DS::goods[posi.x][posi.y];
    bool found = tmp.value > 0 && (tmp.remain_frame > length+2) && (tmp.lock==-1);
    return found;
}

bool get_revolved_berth_id::isValidMove(Position posi, char map[][200]) {
    bool block = (blocked.find(map[posi.x][posi.y]) != blocked.end());
    bool allow = (allowed.find(map[posi.x][posi.y]) != allowed.end());
    return allow && !visited[posi.x][posi.y] && !block;
//    return !block2 && !visited[posi.x][posi.y] && !block;
}

int get_revolved_berth_id::find_goods(Position posi_berth, char map[][200], int max_dep){
    char label = '!';
    int goods_cnt = 0;
    int rows = 200;
    int cols = 200;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            visited[i][j] = false;
        }
    }

    queue<pair<Position, int>> q;

    q.push(make_pair(posi_berth, 1));
//    base_DS::connection[posi_berth.x][posi_berth.y] = label;
    visited[posi_berth.x][posi_berth.y] = true;
    while (!q.empty()) {
        Position current = q.front().first;
        int now_dep = q.front().second;
        if (now_dep > max_dep){
            break;
        }
        q.pop();

        if (isGoods(current, 400)){
            goods_cnt++;
        }

        // 尝试四个方向移动
        for (auto direction: directions) {
            Position attempt = Position(current, direction);
            if (map[attempt.x][attempt.y] == 'B' && now_dep > 3) {
                return goods_cnt;
            }
            if (isValidMove(attempt, map)) {
                q.push(make_pair(attempt, now_dep + 1));
//                map[attempt.x][attempt.y] = label;
                visited[attempt.x][attempt.y] = true;
            }
        }
    }
    return goods_cnt;
}

void get_revolved_berth_id::revolved_berth(vector<Berth> berth, vector<Robot> robot) {
    // 根据上一次分配结果轮转
    vector<int> former_assign(base_DS::berth.size(), 0);
    for(int i=0; i<base_DS::robot_num; i++){
        former_assign[base_DS::berth_assign[i]]++;
    }

    // 搜索港口可达货物数

    vector<int> berth_index(berth.size());

    iota(berth_index.begin(), berth_index.end(), 0);
    sort(berth_index.begin(), berth_index.end(), [&berth](int i1, int i2) {
        return berth[i1].value > berth[i2].value;
    });

    vector<float> berth_range(berth.size(), 0);   // 存储港口可达点数量
    char map[base_DS::N][base_DS::N];
    for (int i=0; i<base_DS::N; i++){
        memcpy(map[i], base_DS::ch[i], base_DS::N);
    }

    double range_sum = 0;
    // BFS | add +
    for (int i=0; i<berth_index.size(); i++) {
        // 从港口各角出发
        Area now_area = berth[berth_index[i]].area;
        berth_range[i] += (float)find_goods(Position(now_area.x_hi, now_area.y_hi),
                                                 map, berth[i].R);
        berth_range[i] += (float)find_goods(Position(now_area.x_hi, now_area.y_lo),
                                                 map, berth[i].R);
        berth_range[i] += (float)find_goods(Position(now_area.x_lo, now_area.y_hi),
                                                 map, berth[i].R);
        berth_range[i] += (float)find_goods(Position(now_area.x_lo, now_area.y_lo),
                                                 map, berth[i].R);
        range_sum += berth_range[i];
    }

    for(int i=0; i<berth.size(); i++){
        berth_range[i] = berth_range[i] / range_sum * robot.size();
    }


    vector<int> assign(robot.size());
    vector<int> berth_range_index(berth_range.size());

    iota(berth_range_index.begin(), berth_range_index.end(), 0);
    sort(berth_range_index.begin(), berth_range_index.end(), [&berth_range](int i1, int i2) {
        return berth_range[i1] > berth_range[i2];
    });


    int space = 0;
    int robot_cnt = 0;
    // 分配方法
    ShortestPathGetter getter;
    // todo : change top N
    for (int i=0; i<berth_range_index.size();){
        if(robot_cnt == robot.size())break;
        int connect_flag = getter.shortestPath(robot[robot_cnt].posi,berth[berth_range_index[i]].leftupper,0).first;    //机器人与港口
        if(connect_flag == -1){
            assign[robot_cnt++] = 0;
            continue;
        }
        if(space >= 1 && (space>berth_range[berth_range_index[i]] || space>=max_robot_per_berth)){
            i++;
            space = 0;
            continue;
        }
        assign[robot_cnt++] = berth_range_index[i];
        space++;
    }

//    for (auto i : assign) {
//        cout << i << "; ";
//    }
//    cout << endl;

    for(int i=0; i<robot.size(); i++){
        robot[i]._set_destinations({berth[assign[i]]});
    }
}
