//#include <climits>
//#include "update.h"
//#include "../algorithm/get_shortest_path.h"
//#include "choose_a_berth.h"
//#include "choose_a_goods.h"
//#include "../data_structures/data_structure.h"
//<<<<<<< HEAD
//#include "should_go_now.h"
//void update (Robot &robot){
//    /*
//        更新机器人状态
//    */
//   //!!!!!!!!!!!!!!!!!注意 该版本没有实现碰撞处理操作
//   Robot rb = robot;
//   if(rb.penalty != 0){//碰撞状态，优先处理
//        rb.penalty--;
//        return ;
//   }
//   if(rb.goods == 1){//载货状态
//        if(rb.decide_to_unload()){//当前已经走到卸货点，执行卸货操作。
//=======
//void update (Robot &robot) {
//    /*
//        更新机器人状态
//    */
//    Robot rb = robot;
//    if (rb.penalty != 0) {//碰撞状态，优先处理
//        rb.penalty--;
//        return;
//    }
//
//    if (rb.goods == 1) {//载货状态
//        if (rb.decide_to_unload()) {//当前已经走到卸货点，执行卸货操作。
//>>>>>>> 442ed900a3df0857703908fb0df34ec490c2711c
//            rb.goods = 0;//卸货
//            rb.current_dest = rb.reset();//重新设置目标点。
//            ShortestPathGetter shetter;
//            auto get_path = shetter.shortestPath(rb.posi, rb.current_dest);
//            if (get_path.first != -1) {//路径可达
//                rb.path = get_path.second;
//                Position next_posi = rb.path.front();
//<<<<<<< HEAD
//                if(rb.will_hit()){//碰撞处理
//                    rb.path.pop();
//                    rb.posi = next_posi;
//                }
//                else{
//=======
//                if (rb.will_hit()) {//碰撞处理
//
//                } else {
//>>>>>>> 442ed900a3df0857703908fb0df34ec490c2711c
//                    rb.path.pop();
//                    rb.posi = next_posi;
//                }
//            } else {
//                rb.current_dest = rb.birth;
//                rb.path = shetter.shortestPath(rb.posi, rb.current_dest).second;
//                Position next_posi = rb.path.front();
//                rb.path.pop();
//                rb.posi = next_posi;
//            }
//<<<<<<< HEAD
//        }
//        else{ //未走到卸货点
//            if(rb.will_hit()){//碰撞检测
//                Position next_posi = rb.path.front();
//                rb.path.pop();
//                rb.posi = next_posi;
//            }
//            else{
//=======
//        } else { //未走到卸货点
//            if (rb.will_hit()) {//碰撞检测
//
//            } else {
//>>>>>>> 442ed900a3df0857703908fb0df34ec490c2711c
//                Position next_posi = rb.path.front();
//                rb.path.pop();
//                rb.posi = next_posi;
//            }
//        }
//    } //载货状态判断结束
//<<<<<<< HEAD
//    else{ //空闲状态
//        if(rb.decide_to_load()){//当前已经走到载货点，执行载货操作。
//=======
//    else { //空闲状态
//        if (rb.decide_to_load()) {//当前已经走到载货点，执行卸货操作。
//>>>>>>> 442ed900a3df0857703908fb0df34ec490c2711c
//            rb.goods = 1;//载货
//            auto res = choose_a_berth(rb);
//            int berth_id = res.first;
//            if (berth_id == -1) {//没有可达的港口
//
//            }
//            rb.current_dest = base_DS::berth[res.first].leftupper;//重新设置目标点。
//            ShortestPathGetter shetter;
//            auto get_path = shetter.shortestPath(rb.posi, rb.current_dest);
//            if (get_path.first != -1) {//路径可达
//                rb.path = get_path.second;
//                Position next_posi = rb.path.front();
//<<<<<<< HEAD
//                if(rb.will_hit()){//碰撞处理 暂时不予特殊处理，就正常移动
//                    rb.path.pop();
//                    rb.posi = next_posi;
//                }
//                else{
//                    rb.path.pop();
//                    rb.posi = next_posi;
//                }
//            }
//            else{ //路径不可达 说明没有港口可以进行运货
//=======
//                if (rb.will_hit()) {//碰撞处理
//
//                } else {
//                    rb.path.pop();
//                    rb.posi = next_posi;
//                }
//            } else {
//>>>>>>> 442ed900a3df0857703908fb0df34ec490c2711c
//                rb.current_dest = rb.birth;
//                rb.path = shetter.shortestPath(rb.posi, rb.current_dest).second;
//                Position next_posi = rb.path.front();
//                rb.path.pop();
//                rb.posi = next_posi;
//            }
//<<<<<<< HEAD
//        }
//        else{ //未走到卸货点
//            if(rb.will_hit()){//碰撞检测
//                Position next_posi = rb.path.front();
//                rb.path.pop();
//                rb.posi = next_posi;
//            }
//            else{ //
//=======
//        } else { //未走到卸货点
//            if (rb.will_hit()) {//碰撞检测
//
//            } else {
//>>>>>>> 442ed900a3df0857703908fb0df34ec490c2711c
//                Position next_posi = rb.path.front();
//                rb.path.pop();
//                rb.posi = next_posi;
//            }
//        }
//    }//空闲状态怕判断结束
//
//   return ;
//}//robot update结束
//
//void update(Boat &boat){
//    if(boat.status == 0){//运输状态不做处理
//        return ;
//    }
//<<<<<<< HEAD
//    if(boat.status == 1){
//        if(boat.pos == -1){//刚刚送达货物，现在需要选择一个港口
//            int target_berth_index = choose_a_berth(boat);
//            if(target_berth_index == -1){//没有合适的港口，继续在虚拟点等待
//                return ;
//            }
//            else{ //有合适的港口,进入排队
//                boat.status = 2;
//                boat.pos =target_berth_index;
//                base_DS::berth[target_berth_index].queue_boats.push_back(pair(boat,1));
//            }
//        }
//        else{//处于装货状态
//            if(should_go_now(boat)){//判断该出发了
//                boat.status = 0;
//                base_DS::berth[boat.pos].queue_boats.erase(base_DS::berth[boat.pos].queue_boats.begin()); //出队
//                boat.pos = -1;
//            }
//            else{//继续装货
//                return ;
//            }
//        }
//    }
//    if(boat.status == 2){//队列等待状态
//        if(base_DS::berth[boat.pos].queue_boats.size() <= 1){ //当等待队列数目等于1时说明轮到自己，这里还是有问题的，没有办法判断码头的等待队列的某个船是否就是我们现在处理的这个船
//        //后面最好给船对象增加一个属性id来进行区分
//            boat.status == 1;//变为载货状态
//        }
//    }
//    return ;
//=======
//    int max_volume = INT_MAX;
//    int best_boat_index = 0;
//    for (int i = 0; i < base_DS::boat.size(); i++) {
//        if (base_DS::boat[i].num < max_volume) {
//            max_volume = base_DS::boat[i].num;
//            best_boat_index = i;
//        }
//    }
//>>>>>>> 442ed900a3df0857703908fb0df34ec490c2711c
//}