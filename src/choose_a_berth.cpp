//
// Created by Administrator on 2024/3/7.
//

#include <algorithm>
#include "choose_a_berth.h"
#include "get_shortest_path.h"
#include "Position.h"
#include "data_structure.h"
/*
    返回值代表选择港口的编号,-1代表不选择港口而继续等待
*/
// 暂时只设置为初始化时选择的那个港口
int choose_a_berth(Robot& robot){
    return robot.destinations.front().id;
}
//    std::vector<Berth>& berth = base_DS::berth;
//    //载货状态下的机器人优先选择一个靠近自己的港口进行运输，只考虑最短路径即可
//    //上层判断
////    if(robot.goods != 1){//非载货状态下不需要选择港口
////        return -1;
////    }
//    int min_distance = 40000;
//    int best_berth_id = -1;
//    ShortestPathGetter getter;
//    pair<int, Path>result;
//    for(int i = 0; i < berth.size(); i++){
//        Berth current_berth = berth[i];
//        Position start = robot.posi;//设置起始点为机器人的当前位置
//        Position end = current_berth.leftupper;
//        result = getter.shortestPath(start, end);
//        if (result.first < 0){
//            continue;
//        }else{
//            // 暂时就选一个有效的
//            break;
//        }
//
//        if(result.first < min_distance){
//            min_distance = result.first;//更新最短距离
//            best_berth_id = i;
//        }
//    }
//    //需要设置一个lock状态来保证机器人在去往港口的过程中不需要进行其他操作；
//    // robot.lock = 1;
//    return pair<int, Path>(best_berth_id, result.second);
//}

// 经验上每70帧一个货物
// 排队结束时间短，货物价值大
using un_l_l = unsigned long long;
//bool berth_compare(pair<int, pair<un_l_l ,un_l_l>>& a, pair<int , pair<un_l_l ,un_l_l>>& b) {
//    pair<un_l_l ,un_l_l> a_value = a.second;
//    pair<un_l_l ,un_l_l> b_value = b.second;
//    if (a_value.second < b_value.second){
//        return true;
//    }else if (a_value.second > b_value.second) {
//        return false;
//    }else {
//        return a_value.first > b_value.first;
//    }
//}

double _get_compare_value(pair<queue<int> ,un_l_l>& a_key){
    int mean_value_per_goods = 50;
    int mean_frames_per_goods = 70;
    double value_pending_a = a_key.second*1. / mean_frames_per_goods*1. * mean_value_per_goods;
    for (int i = 0; i < a_key.second; i++){
        if (a_key.first.empty()){
            break;
        }else{
            a_key.first.pop();
        }
    }
    while(a_key.first.empty()){
        value_pending_a += a_key.first.front();
        a_key.first.pop();
    }
    return value_pending_a;
}

bool berth_compare(pair<int, pair<queue<int> ,un_l_l>>& a, pair<int , pair<queue<int> ,un_l_l>>& b) {
    pair<queue<int> ,un_l_l> a_key = a.second; // 需要深拷贝吗
    pair<queue<int> ,un_l_l> b_key = b.second;
    // a_key.first 待载货物价值队列
    // a_key.second 预计耗时
    double value_pending_a = _get_compare_value(a_key);
    double value_pending_b = _get_compare_value(b_key);
    return value_pending_a > value_pending_b;
}

// 没有考虑行船时间
int choose_a_berth(Boat boat){
//    vector<int> berth_id = {0,1,6,8,9};
//    return berth_id[boat.id_boat];
    std::vector<Berth>& berths = base_DS::berth;
    std::vector<Boat>& boats = base_DS::boat;

    std::vector<pair<int, pair<queue<int> ,un_l_l>>> berth_goods_boats_num(berths.size(), {0, {{}, 0}});
    int capacity = boats.front().capacity;

    if(boat.status == 0 && boat.id_dest_in_plan == -1){//回城船，目前不进行分配
        return -1;
    }

    for(int i = 0; i < berths.size(); i++){
        berth_goods_boats_num[i].first = i;
        // 统计待载货物价值
        queue<int> queue_i = berths[i].queue_goods_value;
        berth_goods_boats_num[i].second.first = queue_i;
        // 统计预计耗时
        berth_goods_boats_num[i].second.second += berths[i].queue_boats.size()*capacity;
        if (berths[i].id_boat_in_berth != -1){
            berth_goods_boats_num[i].second.second += (capacity-boats[berths[i].id_boat_in_berth].load);
        }
    }

    for (int i = 0; i < boats.size(); i++) {
        if (i != boat.id_boat && boats[i].id_dest_on_the_way != -1) { //其它实际前往该港口的船
            berth_goods_boats_num[boats[i].id_dest_on_the_way].second.second+=capacity;
        }
    }

    for (int i = 0; i < boat.id_boat; i++) {
        if (boats[i].id_dest_in_plan != -1) {  //其它计划前往该港口的船
            berth_goods_boats_num[boats[i].id_dest_in_plan].second.second+=capacity;
        }
    }

    for(int i = 0; i < berths.size(); i++){
        berth_goods_boats_num[i].second.second /= berths[i].loading_speed;
    }

    std::sort(berth_goods_boats_num.begin(), berth_goods_boats_num.end(), berth_compare);
    int choose_berth_index = berth_goods_boats_num[0].first;

//    if (berth_goods_boats_num[0].second.second > 85) return -1;  //选出的港口需要很久才能入港，无需重新调度
    if (berth_goods_boats_num[0].second.first.empty()) return -1;  //所有港口都没有货物,无需调度

    return choose_berth_index;
}

//int choose_a_berth(Boat boat){
////    vector<int> berth_id = {0,1,6,8,9};
////    return berth_id[boat.id_boat];
//    std::vector<Berth>& berths = base_DS::berth;
//    std::vector<Boat>& boats = base_DS::boat;
//
//    std::vector<pair<int, pair<un_l_l ,un_l_l>>> berth_goods_boats_num(berths.size(), {0, {0, 0}});
//    int capacity = boats.front().capacity;
//
//    if(boat.status == 0 && boat.id_dest_in_plan == -1){//回城船，目前不进行分配
//        return -1;
//    }
//
//    for(int i = 0; i < berths.size(); i++){
//        berth_goods_boats_num[i].first = i;
//        // 统计待载货物价值
//        queue<int> queue_i = berths[i].queue_goods_value;
//        while(!queue_i.empty()){
//            berth_goods_boats_num[i].second.first += queue_i.front();
//            queue_i.pop();
//        }
//        // 统计预计耗时
//        berth_goods_boats_num[i].second.second += berths[i].queue_boats.size()*capacity;
//        if (berths[i].id_boat_in_berth != -1){
//            berth_goods_boats_num[i].second.second += (capacity-boats[berths[i].id_boat_in_berth].load);
//        }
//    }
//
//    for (int i = 0; i < boats.size(); i++) {
//        if (i != boat.id_boat && boats[i].id_dest_on_the_way != -1) { //其它前往港口的船
//            berth_goods_boats_num[boats[i].id_dest_on_the_way].second.second+=capacity;
//        }
//    }
//
//    for (int i = 0; i < boat.id_boat; i++) {
//        if (boats[i].id_dest_in_plan != -1) {
//            berth_goods_boats_num[boats[i].id_dest_in_plan].second.second+=capacity;
//        }
//    }
//
//    std::sort(berth_goods_boats_num.begin(), berth_goods_boats_num.end(), berth_compare);
//    int choose_berth_index = berth_goods_boats_num[0].first;
//
//    if (berth_goods_boats_num[0].second.second > 85) return -1;  //选出的港口需要很久才能入港，无需重新调度
//    if (berth_goods_boats_num[0].second.first == 0) return -1;  //所有港口都没有货物,无需调度
//
//    return choose_berth_index;
//}

