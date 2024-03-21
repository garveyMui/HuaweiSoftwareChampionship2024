//
// Created by 梅佳伟 on 2024/3/21.
//

#include "scheduler_boat.h"
#include <algorithm>

#include <utility>
#include <cmath>


scheduler_boat::scheduler_boat(vector<Berth *> berths) {
    this->berths = berths;
    this->first = vector<bool>(5, true);
    this->time2ship1 = vector<vector<int>>(5);
    this->time2ship2 = vector<vector<int>>(5);
    this->time2go = vector<vector<int>>(5);
    this->id_berth_first = vector<int>(5, 0);
    this->id_berth_second = vector<int>(5, 0);
}

bool berth_tt_compare(Berth*a, Berth*b) {
    return a->transport_time < b->transport_time;
}
void scheduler_boat::do_schedule() {
    sort(this->berths.begin(), this->berths.end(), berth_tt_compare);
    for (int i = 0; i < this->boat_num; i++){
        this->id_berth_first[i] = this->berths[i]->id;
        this->id_berth_second[i] = this->berths[this->berth_num-1-i]->id;
    }
    for (int i = 0; i < this->boat_num; i++){
        int period = get_cycle(this->berths[i], this->berths[this->berth_num-1-i]);
        auto loading_times = get_loading_time(this->berths[i], this->berths[this->berth_num-1-i]);
        int go_time1 = this->berths[i]->transport_time;
        int go_time2 = this->berths[this->berth_num-1-i]->transport_time;
        int transfer_time = 500;

        int cycle_num = 15000/period ; // 下取整
        bool remain = false;
        remain = ceil(15000.0/period)>cycle_num;
        if (remain){
            int end = 14999; // 预留一帧
            int start = end%period;
            if(start>(this->berths[i]->transport_time*2)){
                this->time2ship1[i].push_back(1);
                this->time2go[i].push_back(start-this->berths[i]->transport_time);
            }else{
                this->first[i] = false;
            }
            for (int j = 0; j < cycle_num; j++){
                this->time2ship2[i].push_back(start);
                this->time2ship1[i].push_back(start+go_time2+loading_times.second);
                this->time2go[i].push_back(start+go_time2+loading_times.second+transfer_time+loading_times.first);
                start += period;
            }
            // 交错
//            for (int j = 0; j < cycle_num; j++){
//                if (j%2==0){
//                    this->time2ship2[i].push_back(start);
//                    this->time2ship1[i].push_back(start+go_time2+loading_times.second);
//                    this->time2go[i].push_back(start+go_time2+loading_times.second+transfer_time+loading_times.first);
//                }else{
//                    this->time2ship1[i].push_back(start);
//                    this->time2ship2[i].push_back(start+go_time1+loading_times.first);
//                    this->time2go[i].push_back(start+go_time1+loading_times.first+transfer_time+loading_times.second);
//                }
//                start += period;
//            }
//            if (this->berths[i]->loading_speed > this->berths[this->berth_num-1-i]->loading_speed){
//                int end = 14999; // 预留一帧
//                int start = end%period;
//                if(start>(this->berths[i]->transport_time*2)){
//                    this->time2ship1[i].push_back(1);
//                    this->time2go[i].push_back(start-this->berths[i]->transport_time);
//                }else{
//                    this->first[i] = false;
//                }
//                for (int j = 0; j < cycle_num; j++){
//                    if (j%2==0){
//                        this->time2ship2[i].push_back(start);
//                        this->time2ship1[i].push_back(start+go_time2+loading_times.second);
//                        this->time2go[i].push_back(start+go_time2+loading_times.second+transfer_time+loading_times.first);
//                    }else{
//                        this->time2ship1[i].push_back(start);
//                        this->time2ship2[i].push_back(start+go_time1+loading_times.first);
//                        this->time2go[i].push_back(start+go_time1+loading_times.first+transfer_time+loading_times.second);
//                    }
//                    start += period;
//                }
//            }
//            else{
//                int end = 14999; // 预留一帧
//                int start = end%period;
//                if(start>(this->berths[this->berth_num-1-i]->transport_time*2)){
//                    this->time2ship2[i].push_back(1);
//                    this->time2go[i].push_back(start-this->berths[i]->transport_time);
//                    this->first[i] = false;
//                }
//                for (int j = 0; j < cycle_num; j++){
//                    if (j%2==0){
//                        this->time2ship1[i].push_back(start);
//                        this->time2ship2[i].push_back(start+go_time1+loading_times.first);
//                        this->time2go[i].push_back(start+go_time1+loading_times.first+transfer_time+loading_times.second);
//                    }else{
//                        this->time2ship2[i].push_back(start);
//                        this->time2ship1[i].push_back(start+go_time2+loading_times.second);
//                        this->time2go[i].push_back(start+go_time2+loading_times.second+transfer_time+loading_times.first);
//                    }
//                    start += period;
//                }
//            }
        }else{
            // 还没写整除的分支
        }
    }
}

int scheduler_boat::get_cycle(Berth *a, Berth *b) {
    auto loading_times = get_loading_time(a,b);
    int cycle = a->transport_time;
    cycle += loading_times.first;
    cycle += 500;
    cycle += loading_times.second;
    cycle += b->transport_time;
    return cycle;
}

pair<int, int> scheduler_boat::get_loading_time(Berth *a, Berth *b) {
    int l1 = a->loading_speed;
    int l2 = b->loading_speed;
    int t1 = ceil(boat_capacity*l2*1.0/(l1*l2+l1));
    int t2 = ceil(t1*l1*1.0/l2);
    return {t1, t2};
}

int scheduler_boat::choose_a_berth(Boat &boat) {
    if (this->first[boat.id_boat]){
        this->first[boat.id_boat] = false;
        return this->id_berth_first[boat.id_boat];
    }else{
        this->first[boat.id_boat] = true;
        return this->id_berth_second[boat.id_boat];
    }
}

bool scheduler_boat::should_go_now(Boat &boat) {
    if (this->time2go[boat.id_boat].front()==this->frame){
        this->time2go[boat.id_boat].erase(this->time2go[boat.id_boat].begin());
        return true;
    }else{
        return false;
    }
}

bool scheduler_boat::should_ship_now(Boat &boat) {
    if (this->time2ship1[boat.id_boat].front()==this->frame){
        this->time2ship1[boat.id_boat].erase(this->time2ship1[boat.id_boat].begin());
//        boat.id_dest_in_plan = this->id_berth_first[boat.id_boat];
        return true;
    }else if(this->time2ship2[boat.id_boat].front()==this->frame){
        this->time2ship2[boat.id_boat].erase(this->time2ship2[boat.id_boat].begin());
//        boat.id_dest_in_plan = this->id_berth_second[boat.id_boat];
        return true;
    }else{
        return false;
    }
}

void scheduler_boat::update(int id) {
    this->frame = id;
}
