//
// Created by Administrator on 2024/3/7.
//

#include "Berth.h"
#include "data_structure.h"
#include "get_shortest_path.h"

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
    _init_cells();

} // Berth
void Berth::_init_cells(){
    auto getter = ShortestPathGetter();
    getter.set_cells_berth(this->area.center, this->cells, this->connection, this->territories);
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

Path Berth::point2berth(Position point) {
    auto path = ShortestPathGetter::reconstructPath(&this->cells[point.x][point.y], false);
    return path;
}
