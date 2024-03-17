//
// Created by 梅佳伟 on 2024/3/8.
//

#include "get_car_priority.h"
#include "data_structure.h"
double get_car_priority (Robot robot){
    /*
    优先级决定策略：
        1.目前为空并且有目标货物(这么设置是因为防止出现因为等待导致货物消失的问题)
        2.目前有货物
        3.目前无货物且无目标货物
    具体计算方法为：
        如果有货物的优先级计算为：
            货物价值
        如果没有货物且有目标货物：
            货物剩余消失时间+1000+货物价值
        如果没有货物且没有目标货物
            -1
    */
    if (robot.surrounded!=0){
        return -robot.surrounded;
    }
    if(robot.goods == 1){ //有货物
        return robot.goods_value;
    }
    if(robot.goods == 0 && base_DS::goods[robot.current_dest.x][robot.current_dest.y].value != 0){//没有货物且没有目标
        return -0.5;
    }
    else{ //没有货物但有目标
        return 1000 - base_DS::goods[robot.current_dest.x][robot.current_dest.y].remain_frame + base_DS::goods[robot.current_dest.x][robot.current_dest.y].value;
    }
}