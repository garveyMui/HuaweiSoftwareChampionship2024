//
// Created by 梅佳伟 on 2024/3/12.
//

#include "should_ship_now.h"
// 决定是否前往装货泊位
bool should_ship_now(Boat boat){
    // 在虚拟点停靠应前往某泊位
    if(boat.status==1 && boat.id_dest_on_the_way==-1){
        return true;
    }

    // 在排队且新计划港口与现港口不一致
    if(boat.status==2 && boat.id_dest_in_plan!=boat.id_dest_on_the_way){
        return true;
    }

    /**可能有问题**/
    // 船在行驶途中,并且目的地不是虚拟点，可以重置指令去别的港口
//    if (boat.status == 0 && boat.id_dest_on_the_way != -1) {
//        if (boat.id_dest_on_the_way != boat.id_dest_in_plan)    //更改了目的位置
//            return true;
//    }
    return false;
}