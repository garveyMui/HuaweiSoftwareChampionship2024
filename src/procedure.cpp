//
// Created by 梅佳伟 on 2024/3/8.
//

#include "procedure.h"
#include "data_structure.h"
#include "commands.h"
#include <fstream>

// 现在装卸货的时候不会再移动
void Procedure::do_procedure() {
    //对每个机器人输入操作指令
    for(int i = 0; i < base_DS::robot_num; i ++) {
        if (base_DS::robot[i].dead){
            continue;
        }
//        auto start = std::chrono::high_resolution_clock::now(); // 获取当前时间
        // 移动前动作
        if (base_DS::robot[i].decide_to_load() ||
            base_DS::robot[i].decide_to_unload()){
            // 调用函数

            if (base_DS::robot[i].decide_to_load()){
                std::cout << get_GET_command(i);
            }else{
                std::cout << get_PULL_command(i);
            }
        }
//        auto end = std::chrono::high_resolution_clock::now();   // 获取当前时间
//         计算执行时间
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//        std::cout << "1函数执行时间： " << duration.count()/1000.0 << " ms" << std::endl;


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


    for (int i = 0; i < base_DS::boat_num; i++) {
        // 决定是否前往虚拟点
        if (base_DS::scheduler->should_go_now(base_DS::boat[i])) {
            std::cout << get_GO_command(i);
            std::ofstream file("num_go.txt", std::ios::out | std::ios::app);
            file << "id " << base_DS::id << endl;
            file << "boat " << i << endl;
            file << "goods_carried " << base_DS::boat[i].load << endl;
            file << "berth_stayed " << base_DS::boat[i].id_dest_on_the_way << endl;
            file.close();
        } else if (base_DS::scheduler->should_ship_now(base_DS::boat[i])) {
            base_DS::boat[i].id_dest_in_plan = base_DS::scheduler->choose_a_berth(base_DS::boat[i]);
            if (base_DS::boat[i].id_dest_in_plan != -1){
                std::cout << get_SHIP_command(i, base_DS::boat[i].id_dest_in_plan);
                std::ofstream file("ship_id.txt", std::ios::out | std::ios::app);
                file << "id " << base_DS::id << endl;
                file << "boat " << i << endl;
                file << "goods_carried " << base_DS::boat[i].load << endl;
                file << "from " << base_DS::boat[i].id_dest_on_the_way << endl;
                file << "to " << base_DS::boat[i].id_dest_in_plan << endl;
                file.close();
            }
        } else {
            //在装货，或者在途中
        }
    }
}

