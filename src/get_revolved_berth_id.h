//
// Created by Administrator on 2024/3/18.
//
#include "data_structure.h"

#ifndef APPLE_GET_REVOLVED_BERTH_ID_H
#define APPLE_GET_REVOLVED_BERTH_ID_H


class get_revolved_berth_id {
public:
    int revolve_berth_frame = 3000;
    int max_robot_per_berth = 2;
    int change_top_n = 5;

    vector<pair<int,int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    unordered_set<char> blocked = {'*', '#'};
    unordered_set<char> allowed = {'.', 'A'};
    bool visited[200][200];

    get_revolved_berth_id();
    void revolved_berth(vector<Berth> , vector<Robot>);

    bool isGoods(Position posi, int length);
    bool isValidMove(Position posi, char map[][200]);
    int find_goods(Position posi_berth, char map[][200], int max_dep);
};


#endif //APPLE_GET_REVOLVED_BERTH_ID_H
