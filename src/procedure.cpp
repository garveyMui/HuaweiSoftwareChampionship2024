//
// Created by 梅佳伟 on 2024/3/8.
//

#include "procedure.h"
#include "data_structure.h"
#include "commands.h"
#include "should_go_now.h"
#include "choose_a_berth.h"

#include "should_ship_now.h"


// 现在装卸货的时候不会再移动
void Procedure::do_procedure() {
    //对每个机器人输入操作指令
    for(int i = 0; i < base_DS::robot_num; i ++) {
        if (base_DS::robot[i].dead){
            continue;
        }
        // 移动前动作
        if (base_DS::robot[i].decide_to_load() ||
            base_DS::robot[i].decide_to_unload()){
            if (base_DS::robot[i].decide_to_load()){
                std::cout << get_GET_command(i);
            }else{
                std::cout << get_PULL_command(i);
            }
        }
        // 移动动作
        if(base_DS::robot[i].status == 1){
            if (base_DS::safe[i]){
                int move_id = base_DS::robot[i].get_move_id();
                if (move_id == -1){
                    // 空路径异常
//                    std::cout << get_MOVE_command(i, 0);
                }else{
                    std::cout << get_MOVE_command(i, move_id);
                }
            }
        }
        // 移动后动作
        if (base_DS::robot[i].decide_to_load() ||
            base_DS::robot[i].decide_to_unload()){
            if (base_DS::robot[i].decide_to_load()){
                std::cout << get_GET_command(i);
            }else{
                std::cout << get_PULL_command(i);
            }
        }
    }

    for (int i = 0; i < base_DS::boat_num; i++){
        // 决定是否前往虚拟点
        if (should_go_now(base_DS::boat[i])){
            std::cout << get_GO_command(i);
        }else if (should_ship_now(base_DS::boat[i]) ){
            if (base_DS::boat[i].id_dest_in_plan != -1)
                std::cout << get_SHIP_command(i, base_DS::boat[i].id_dest_in_plan);
        }else{
            //在装货，或者在途中
        }

//        if (should_go_now(base_DS::boat[i])){
//            std::cout << get_GO_command(i);
//        }else if (base_DS::boat[i].id_dest==-1){
//            int berth_id = choose_a_berth(base_DS::boat[i]);                //复杂调度策略
////            int berth_id = choose_a_berth(base_DS::boat[i], i);  //简单调度策略
//            if (0 <= berth_id && berth_id < base_DS::berth_num){
//                std::cout << get_SHIP_command(i, berth_id);
//            }
//        }
    }
}