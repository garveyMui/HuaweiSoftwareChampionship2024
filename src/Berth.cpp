//
// Created by Administrator on 2024/3/7.
//

#include "Berth.h"
#include "data_structure.h"
#include "get_shortest_path.h"
#include <chrono>

Berth::Berth(int x, int y, int transport_time, int loading_speed, int id_berth) {
    this->id = id_berth;
    this->leftupper = Position(x, y);
    this->area = Area(x, x+4, y, y+4);
    this->transport_time = transport_time;
    this->loading_speed = loading_speed;
    this->value = (double)loading_speed / transport_time;
    this->num_assigned_robots = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            this->occupied[i][j] = false;
        }
    }
    this->cells = vector<vector<APosition>>(200, vector<APosition>(200));
    this->connection = vector<vector<bool>>(200, vector<bool>(200,false));
    auto start = std::chrono::high_resolution_clock::now(); // 获取当前时间
    _init_cells();                             // 调用函数
    auto end = std::chrono::high_resolution_clock::now();   // 获取当前时间
    // 计算执行时间
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // 输出执行时间
//    std::cout << "1函数执行时间： " << duration.count() << " 微秒" << std::endl;


} // Berth
vector<Area> areas = {Area(0,50,0,100),
                      Area(0,50,100,200),
                      Area(0,50,100,150),
                      Area(50,100,0,100),
                      Area(50,100,100,200),
                      Area(100,150,0,100),
                      Area(100,150,100,200),
                      Area(150,200,0,50),
                      Area(150,200,0,100),
                      Area(150,200,100,200),};
void Berth::_init_cells(){
    auto getter = ShortestPathGetter();
    getter.set_cells_berth(this->area.center, this->cells, this->connection, this->territories);
    this->set_territories(areas[this->id]);
}
Berth::Berth() {

}

bool Berth::in_certain_berth(Position posi) {
    for (int i = 0; i < base_DS::berth_num; i++){
        if (base_DS::berth[i].area.contains(posi)){
            return true;
        }
    }
    return false;
}

Path Berth::berth2point(Position point) {
    auto path = ShortestPathGetter::reconstructPath(&this->cells[point.x][point.y]);
    return path;
}

void Berth::point2berth(Position point, Path &path) {
//    auto start = std::chrono::high_resolution_clock::now(); // 获取当前时间
    ShortestPathGetter::reconstructPath(&this->cells[point.x][point.y], false, path);                     // 调用函数
//        auto end = std::chrono::high_resolution_clock::now();   // 获取当前时间
//     计算执行时间
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//     输出执行时间
//        std::cout << "函数执行时间： " << duration.count()/1000.0 << " 毫秒" << std::endl;
}

Position Berth::get_goods_posi() {
    double max_value = 0;
    Position res;
    for (auto iter = territories.begin(); iter != territories.end(); iter++){
        double cur_value = iter->second/(this->cells[iter->first.x][iter->first.y].level+1.0);
        if(cur_value>max_value){
            max_value = cur_value;
            res = iter->first;
        }
    }
    return res;
}

void Berth::reset_goods(Position posi) {
    if (this->territories.find(posi) == this->territories.end()){
        return;
    }else{
        this->territories.at(posi) = 0;
    }
}

void Berth::set_goods(Position posi, int value) {
    if (this->territories.find(posi) == this->territories.end()){
        return;
    }else{
        this->territories.at(posi) = value;
    }
}

void Berth::set_territories(Area area) {
//    this->territories.clear();
    vector<Position> pending2erase;
    for (auto iter:this->territories){
        if (!area.contains(iter.first)){
            pending2erase.push_back(iter.first);
        }
    }
    for (auto &posi:pending2erase){
        this->territories.erase(posi);
    }
//    for(int x = area.x_lo; x < area.x_hi; x++){
//        for (int y = area.y_lo; y < area.y_hi; y++){
//            this->territories.insert({Position(x,y),0});
//        }
//    }
}
