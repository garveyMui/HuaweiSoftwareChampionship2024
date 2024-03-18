//
// Created by Administrator on 2024/3/18.
//

#include "get_init_berth_id.h"

#include "get_shortest_path.h"
#include "data_structure.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstring>
using namespace std;

/*
 * 根据map的特点选出若干港口给十个机器人作为初始化负责港口
 * 可以对每个港口进行BFS，确定一定半径内可达的点的个数
 * 由于每个港口之间可能有交叉，可以按照港口效率value进行排序确定交叉位置所属港口
 * 排序后按照可达点数一定阈值与地图连通性分配给若干机器人
 * 返回值应该为一个size()==10的整形数组
 */

vector<pair<int,int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
unordered_set<char> blocked = {'*', '#'};
unordered_set<char> allowed = {'.', 'A'};
bool visited[200][200];

bool isValidMove(bool visited[200][200], Position posi, char map[][200]) {
    bool block = (blocked.find(map[posi.x][posi.y]) != blocked.end());
    bool allow = (allowed.find(map[posi.x][posi.y]) != allowed.end());
    return allow && !visited[posi.x][posi.y] && !block;
//    return !block2 && !visited[posi.x][posi.y] && !block;
}

int draw_berth_area(Position posi_berth, char map[][200], int max_dep){
    char label = '!';
    int range_cnt = 0;
    int rows = 200;
    int cols = 200;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            visited[i][j] = false;
        }
    }

    queue<pair<Position, int>> q;

    q.push(make_pair(posi_berth, 1));
    base_DS::connection[posi_berth.x][posi_berth.y] = label;
    visited[posi_berth.x][posi_berth.y] = true;
    while (!q.empty()) {
        Position current = q.front().first;
        int now_dep = q.front().second;
        if (now_dep > max_dep){
            break;
        }
        q.pop();

        // 尝试四个方向移动
        for (auto direction: directions) {
            Position attempt = Position(current, direction);
            if (map[attempt.x][attempt.y] == 'B' && now_dep > 3) {
                return range_cnt;
            }
            if (isValidMove(visited, attempt, map)) {
                q.push(make_pair(attempt, now_dep + 1));
                map[attempt.x][attempt.y] = label;
                visited[attempt.x][attempt.y] = true;
                range_cnt++;
            }
        }
    }
    return range_cnt;
}

vector<int> get_init_berth_id(vector<Berth> berth, vector<Robot> robot){
    const int max_robot_per_berth = 2; // 临时定义

    vector<float> berth_range(berth.size(), 0);   // 存储港口可达点数量
    vector<int> berth_index(berth.size());

    iota(berth_index.begin(), berth_index.end(), 0);
    sort(berth_index.begin(), berth_index.end(), [&berth](int i1, int i2) {
        return berth[i1].value > berth[i2].value;
    });

    // 临时地图
    char map[base_DS::N][base_DS::N];
    for (int i=0; i<base_DS::N; i++){
        memcpy(map[i], base_DS::ch[i], base_DS::N);
    }

    double range_sum = 0;
    // BFS | add +
    for (int i=0; i<berth_index.size(); i++) {
        // 从港口各角出发
        Area now_area = berth[berth_index[i]].area;
        berth_range[i] += (float)draw_berth_area(Position(now_area.x_hi, now_area.y_hi),
                                          map, berth[i].R);
        berth_range[i] += (float)draw_berth_area(Position(now_area.x_hi, now_area.y_lo),
                                          map, berth[i].R);
        berth_range[i] += (float)draw_berth_area(Position(now_area.x_lo, now_area.y_hi),
                                          map, berth[i].R);
        berth_range[i] += (float)draw_berth_area(Position(now_area.x_lo, now_area.y_lo),
                                          map, berth[i].R);
        range_sum += berth_range[i];
    }

//    for (int i : berth_range) {
//        cout << i << "; ";
//    }
//    cout << endl;

    for(int i=0; i<berth.size(); i++){
        berth_range[i] = berth_range[i] / range_sum * robot.size();
    }

    vector<int> assign(robot.size());
    vector<int> berth_range_index(berth_range.size());

    iota(berth_range_index.begin(), berth_range_index.end(), 0);
    sort(berth_range_index.begin(), berth_range_index.end(), [&berth_range](int i1, int i2) {
        return berth_range[i1] > berth_range[i2];
    });

//    for (auto i : berth_range_index) {
//        cout << i << "; ";
//    }
//    cout << endl;

    int space = 0;
    int robot_cnt = 0;
    // 分配方法
    for (int i=0; i<berth_range_index.size();){
        if(robot_cnt == robot.size())break;
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
    return assign;
}
