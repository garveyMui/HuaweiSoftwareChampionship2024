//
// Created by Administrator on 2024/3/7.
//

#include "choose_a_goods.h"
#include "get_shortest_path.h"
#include "data_structure.h"
#include<map>
/*
 * 从robot负责的Area中选择一个goods
 */

/*
输入参数解释：
robot:机器人
gds：二维数组，表示坐标点位置是否有货物，大于0表示有货物且该值等于货物价值，-1表示没有货物
goods_map:哈希表，存储位置坐标对：Goods对象，其中总有一个坐标对为<-1,-1>的键对应一个虚拟的Goods对象表示视野范围内没有可选择的Goods
*/
Position choose_a_goods(Robot &robot) {
    // 如果货太多就从自身开始遍历 否则从负责区域遍历
    // 都没有满足条件的就根据距离从港口和区域中心选一个远的返回，并且从视野中计算一个曼哈顿距离最小的角去计算最短路径
    // 上层开始阶段间隔更新，每次只更新一个机器人的目标，后面每次到到达了之前的目标再进一步更新

    // robot.posi_goods_in_area的逻辑还没写呢 ，应该创建一个robot的方法然后再input货物更新那里调用

    int min_dist = 400; // 对角最远距离
    Position result;
    // 视野里有
    if (0 < robot.posi_goods_in_view.size()){
        for (auto posi_goods:robot.posi_goods_in_view){
            if (posi_goods-robot.posi < min_dist && base_DS::connection[posi_goods.x][posi_goods.y]){
                min_dist = posi_goods-robot.posi;
                result = posi_goods;
            }
        }
    }
    if (Position(-1,-1)!=result){
        return result;
    }

    // 负责区域里有 这个区域的还没写
    min_dist = 400;
    result = Position(-1,-1);
    if (0 < robot.posi_goods_in_area.size()){
        for (auto posi_goods:robot.posi_goods_in_area){
            if (posi_goods-robot.posi < min_dist && base_DS::connection[posi_goods.x][posi_goods.y]){
                min_dist = posi_goods-robot.posi;
                result = posi_goods;
            }
        }
    }
    if (Position(-1,-1)!=result){
        return result;
    }

    // 去港口或者在港口附近逗留
    if (100 < (robot.posi-robot.destinations.front().leftupper)){
        return robot.destinations.front().leftupper;
    }else{ // 不加随机
//        vector<Direction> ds = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
//        for (Direction d : ds){
//            Position attempt = Position(robot.posi, d);
//            if (base_DS::connection[attempt.x][attempt.y]){
//                return attempt;
//            }
        }
    return {-1,-1}; // 这条永远不会执行到
    }
//    // 没事做就去远一点的地方
//    if ((robot.posi-robot.birth) < (robot.posi-robot.destinations.front().leftupper)){
//        return robot.destinations.front().leftupper;
//    }else{
//        return robot.birth;
//    }


//pair<Position, Path> choose_a_goods(Robot& robot){
//    //已装货的话不会调用这个函数
////    if(robot.goods == 1){//已经装货
////        return robot.posi; //
////    }
//    ShortestPathGetter getter;
//    int max_val = 0;
//    int min_path_len = 40000;
//    // 这个地方在一定帧数之前应该改成往港口走
//    Position target(-1,-1);
//    auto res = getter.shortestPath(robot.posi, target, 0);
//    if () {
//    }
////    int x_lo = std::max(robot.posi.x-robot.view,0);
////    int x_hi = std::min(200, robot.posi.x+robot.view);
////    int y_lo = std::max(robot.posi.y-robot.view,0);
////    int y_hi = std::min(200, robot.posi.y+robot.view);
////    Area view_area(x_lo,x_hi,y_lo,y_hi);
////    for(auto posi_goods_i:base_DS::posi_goods){
////        if(!view_area.contains(posi_goods_i)){
////            continue;
////        }
////        Goods& gd = base_DS::goods[posi_goods_i.x][posi_goods_i.y];
////        auto res = getter.shortestPath(robot.posi,gd.posi);
////        if(res.first > gd.remain_frame){//时间不够，不可达
////            continue;
////        }
////        target = gd.posi;
////        return pair<Position, Path>(target, res.second);
//        // 后面暂时不用看看时间
//        //可达
////        if(gd.value > max_val){//价值更大
////            target = gd.posi;
////            min_path_len = getter.shortestPath(robot.posi,gd.posi).first;
////        }
////        if(gd.value == max_val && getter.shortestPath(robot.posi,gd.posi).first < min_path_len){//等价但距离更短
////            target = gd.posi;
////            min_path_len = getter.shortestPath(robot.posi,gd.posi).first;
////        }
////    }
//
//    // 这个地方可以改成从中间开始遍历 或者用货物位置遍历
////    for(int i = std::max(robot.posi.x-robot.view,0); i < std::min(200, robot.posi.x+robot.view); i++){
////        for(int j = std::max(robot.posi.y-robot.view,0); j < std::min(200, robot.posi.y+robot.view); j++){
////            if(base_DS::goods[i][j].value <= 0){//一点处没有货物，跳过操作
////                continue;
////            }
////            Goods gd = base_DS::goods[i][j];
////            if(getter.shortestPath(robot.posi,gd.posi).first > gd.remain_frame){//时间不够，不可达
////                continue;
////            }
////            //可达
////            if(gd.value > max_val){//价值更大
////                target = gd.posi;
////                min_path_len = getter.shortestPath(robot.posi,gd.posi).first;
////            }
////            if(base_DS::goods[i][j].value == max_val && getter.shortestPath(robot.posi,gd.posi).first < min_path_len){//等价但距离更短
////                target = gd.posi;
////                min_path_len = getter.shortestPath(robot.posi,gd.posi).first;
////            }
////        }
////    }
//
//    //没找到货物的时候默认返回不存在的位置
//    return pair<Position, Path>(Position(-1,-1), queue<Position>());
//
//}