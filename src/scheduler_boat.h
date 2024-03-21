//
// Created by 梅佳伟 on 2024/3/21.
//

#ifndef MAIN_SCHEDULER_BOAT_H
#define MAIN_SCHEDULER_BOAT_H
#include <vector>
#include <stack>
#include "Berth.h"

using namespace std;
class scheduler_boat {
public:
    vector<Berth*> berths;
    vector<bool> first;
    vector<vector<int>> time2ship1;
    vector<vector<int>> time2ship2;
    vector<vector<int>> time2go;
    vector<int> id_berth_first;
    vector<int> id_berth_second;
    scheduler_boat(vector<Berth*> berths);
    scheduler_boat();
    void do_schedule();
    int get_cycle(Berth*a, Berth*b);
    static pair<int,int> get_loading_time(Berth*a, Berth*b);
    int choose_a_berth(Boat& boat);
    bool should_go_now (Boat& boat);
    bool should_ship_now(Boat& boat);
    int frame;
    int berth_num = 10;
    int boat_num = 5;
    static int boat_capacity;
    void update(int id);
};


#endif //MAIN_SCHEDULER_BOAT_H
