//
// Created by 梅佳伟 on 2024/3/8.
//

#include "should_go_now.h"
#include "Berth.h"
#include "data_structure.h"
/*
 *  0表示移动(运输)中
 *  1表示正常运行状态(即装货状态或运输完成状态)
 *  2表示泊位外等待状态
 */
bool should_go_now (Boat boat){
    int frame = base_DS::id;
    std::vector<Berth>& berths = base_DS::berth;
    /*
        决定是否进行船只移动
        移动准则为：
            1.剩余时间帧等于需要到达虚拟点的时间，则立刻执行运输
            2.当前轮船货物数量已满
            3.当前港口已经无货物可装载
    */

    if(boat.status != 1){//非停靠状态 不需要移动
        return false;
    }
    if(boat.id_dest_on_the_way == -1){//位于虚拟点
        return false;
    }
    if(15000 - frame == berths[boat.id_dest_on_the_way].transport_time + 1){//剩余时间不足
        return true;
    }
    if(boat.load >= boat.capacity){ //货物已满载
        return true;
    }
//    if(berths[boat.id_dest_on_the_way].queue_goods_value.size() == 0 && boat.load > 0){//当前港口已无货物可装载并且装有货物
//        return true;
//    }
    return false;


}