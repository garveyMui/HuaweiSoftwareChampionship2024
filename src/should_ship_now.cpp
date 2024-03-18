//
// Created by 梅佳伟 on 2024/3/12.
//

#include "should_ship_now.h"
#include "data_structure.h"
// 决定是否前往装货泊位
bool should_ship_now(Boat& boat){
    // 在虚拟点停靠应前往某泊位
    if(boat.status==1 && boat.id_dest_on_the_way==-1){
        return true;
    }

    // 在排队且新计划港口与现港口不一致
    if(boat.status==2 && boat.id_dest_in_plan!=boat.id_dest_on_the_way){
        return true;
    }

    // 计划泊位有更多货物，且移动消耗是值得的
    int move_between_berths = 500;
    int mean_value_per_goods = 50;
    int mean_frames_per_goods = 70;
    int capacity = boat.capacity;
    if (boat.status==1 && boat.last_status==1){
        int num_goods_current_berth = base_DS::berth[boat.id_dest_on_the_way].queue_goods_value.size();
        if (num_goods_current_berth+move_between_berths/mean_frames_per_goods >= capacity-boat.load){
            return false;
        }
        int num_goods_plan_berth = base_DS::berth[boat.id_dest_in_plan].queue_goods_value.size();
//        if (num_goods_plan_berth >= num_goods_current_berth+move_between_berths/base_DS::berth[boat.id_dest_in_plan].loading_speed){
//            return true;
//        }
        int delta2image_point = base_DS::berth[boat.id_dest_in_plan].transport_time-base_DS::berth[boat.id_dest_on_the_way].transport_time;
        if (num_goods_plan_berth > capacity-boat.load+(move_between_berths+delta2image_point)/mean_frames_per_goods){
            return true;
        }
//        if (base_DS::id >= 13000 && !boat.final_change){
//            boat.final_change = true;
//            return true;
//        }
    }


    /**可能有问题**/
    // 船在行驶途中,并且目的地不是虚拟点，可以重置指令去别的港口
//    if (boat.status == 0 && boat.id_dest_on_the_way != -1) {
//        if (boat.id_dest_on_the_way != boat.id_dest_in_plan)    //更改了目的位置
//            return true;
//    }
    return false;
}