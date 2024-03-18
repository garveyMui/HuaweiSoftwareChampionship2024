//
// Created by Administrator on 2024/3/18.
//

#include "get_init_berth_id.h"
/*
 * 根据map的特点选出若干港口给十个机器人作为初始化负责港口
 * 可以对每个港口进行BFS，确定一定半径内可达的点的个数
 * 由于每个港口之间可能有交叉，可以按照港口效率value进行排序确定交叉位置所属港口
 * 排序后按照可达点数一定阈值与地图连通性分配给若干机器人
 * 返回值应该为一个size()==10的整形数组
 */
vector<int> get_init_berth_id(){

}