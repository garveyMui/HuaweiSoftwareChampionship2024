//
// Created by Administrator on 2024/3/7.
//

#include "get_shortest_path.h"
#include "data_structure.h"
#include <set>
#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include <climits>


using namespace std;


ShortestPathGetter::ShortestPathGetter() {
    this->blocked.insert('*');
    this->blocked.insert('#');
}
// 用于A*算法优先级
struct CompareAPosition {
    bool operator()(const APosition& a, const APosition& b) const {
        return a.f > b.f;
    }
};

void ShortestPathGetter::set_cells_berth(Position posi_berth, vector<vector<APosition>>& cells,
                                         vector<vector<bool>>& connection, unordered_map<Position, int, PositionHash>& territories){
    int rows = 200;
    int cols = 200;

    // 初始化 visited 数组
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            visited[i][j] = false;
            cells[i][j].x = i;
            cells[i][j].y = j;
        }
    }

    queue<Position> q;

    q.push(posi_berth);
    connection[posi_berth.x][posi_berth.y] = true;
    visited[posi_berth.x][posi_berth.y] = true;
    cells[posi_berth.x][posi_berth.y].level = 0;
    territories.insert({posi_berth,0});

    while (!q.empty()) {
        Position current = q.front();
        q.pop();
        // 尝试四个方向移动
        for (auto direction: this->directions) {
            Position attempt = Position(current, direction);
            if (isValidMove(visited, attempt)) {
                q.push(attempt);
                connection[attempt.x][attempt.y] = true;
                visited[attempt.x][attempt.y] = true;
                cells[attempt.x][attempt.y].parent = &cells[current.x][current.y];
                cells[attempt.x][attempt.y].level = cells[current.x][current.y].level+1;
                territories.insert({attempt,0});
            }
        }
    }
}

void ShortestPathGetter::set_connected_set(Position posi_robot, int tag){
    int rows = 200;
    int cols = 200;


    // 初始化 visited 数组
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            visited[i][j] = false;

        }
    }

    queue<Position> q; // 存储 (x, y, path)

    q.push(posi_robot);
    base_DS::connection[posi_robot.x][posi_robot.y] = tag;
    visited[posi_robot.x][posi_robot.y] = true;

    while (!q.empty()) {

        Position current = q.front();
        q.pop();
        // 尝试四个方向移动
        for (auto direction: this->directions) {
            Position attempt = Position(current, direction);

            if (isValidMove(visited, attempt)) {
                q.push(attempt);
                base_DS::connection[attempt.x][attempt.y] = tag;
                visited[attempt.x][attempt.y] = true;
            }
        }
    }
}


// 检查移动是否有效
bool ShortestPathGetter::isValidMove(vector<vector<bool>> &visited, Position posi) {
    bool block = (this->blocked.find(posi.get_symbol()) != this->blocked.end());
    return this->space.contains(posi) && !visited[posi.x][posi.y] && !block;
}

bool ShortestPathGetter::isValidMove(unordered_set<Position, PositionHash> visited, Position posi) {
    bool block = (this->blocked.find(posi.get_symbol()) != this->blocked.end());
    return (this->space.contains(posi)) && (visited.find(posi)==visited.end()) && (!block);
}
// 这个指针使用方法不记得对不对
bool ShortestPathGetter::isValidMove(bool visited[200][200], Position posi) {
    bool block = (this->blocked.find(posi.get_symbol()) != this->blocked.end());
    return this->space.contains(posi) && !visited[posi.x][posi.y] && !block;
}

// 0找berth 1找货物
bool ShortestPathGetter::find_it(Position posi, int type, Position current_dest, int length){
    // bfs不用这个
//    if (!base_DS::connected_set[posi.x][posi.y]){
//        return false;
//    }
    if (type == -1){
        return posi==current_dest;
    }
    if (type == 0){
        return posi.get_symbol()=='B';
    }
    if (type == 1){
        const Goods& tmp = base_DS::goods[posi.x][posi.y];
        bool lock = false;
        for (int i = 0; i < base_DS::robot_num; i++){
            if (base_DS::robot[i].current_dest==posi){
                lock = true;
                break;
            }
        }
        bool found = tmp.value > 0 && (tmp.remain_frame > length+2) && (!lock);
        return found;
    }
    return false; //都没有的话上层可能死循环，可用于定位逻辑错误
}

/**指针实现BFS**/
// 0找berth 1找货物 -1找current_dest
pair<int, queue<Position>> ShortestPathGetter::shortestPath(Position start, Position& end, int type) {
    if (end!=Position(-1,-1)){
        if (this->blocked.find(end.get_symbol()) != this->blocked.end() && type==-1) {

            return {-1, queue<Position>()};
        }
    }

    // 初始化 visited 数组
    for (int i = 0; i < 200; i++){
        for (int j = 0; j < 200; j++){
            visited[i][j] = false;
            cells[i][j].parent = nullptr;
        }
    }

    queue<Position> q;
    q.push(start);
    visited[start.x][start.y] = true;

    int max_level = 60; // 寻找高价值货物的超参数
    // 0:  195282
    // 5:  194240
    // 10: 194932
    // 20: 193612
    // 30: 192068
    // 40: 191278
    int max_value = 100; // 寻找货物时记录
//    int max_value = 0; // 寻找货物时记录
    double value_per_level = -1;
    Position max_value_posi;
    int cur_level = 0;
    Position mark = q.front();

    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        if (find_it(current, type, end, cur_level)) {
            if (type != 1){
                auto res = reconstructPath(&cells[current.x][current.y]);
                end = current;
                return {res.size() - 1, res}; // 返回路径长度和路径
            }else{
                int cur_value = base_DS::goods[current.x][current.y].value;
                // 一定层数内最大价值
//                if (cur_value > max_value){
//                    max_value = cur_value;
//                    max_value_posi = current;
//                }
//                if (cur_level>=max_level){
//                    auto res = reconstructPath(&cells[max_value_posi.x][max_value_posi.y]);
//                    end = max_value_posi;
//                    return {res.size() - 1, res}; // 返回路径长度和路径
//                }
                // 价值高于一定阈值的第一个
                if (cur_value >= max_value){
                    max_value_posi = current;
                    auto res = reconstructPath(&cells[max_value_posi.x][max_value_posi.y]);
                    end = max_value_posi;
                    return {res.size() - 1, res}; // 返回路径长度和路径
                }
                // 一定层数内最大(价值/距离)
//                if ((cur_value*1.)/(cur_level*1.) >= value_per_level){
//                    max_value_posi = current;
//                }
//                if (cur_level>=max_level){
//                    auto res = reconstructPath(&cells[max_value_posi.x][max_value_posi.y]);
//                    end = max_value_posi;
//                    return {res.size() - 1, res}; // 返回路径长度和路径
//                }
            }
        }

        // 尝试四个方向移动
        for (auto direction: this->directions) {
            Position attempt = Position(current, direction);
            if (isValidMove(visited, attempt)) {
                cells[attempt.x][attempt.y].parent = &cells[current.x][current.y];
                q.push(attempt);
                visited[attempt.x][attempt.y] = true;
            }
        }
        if (current==mark){
            mark = q.back();
            cur_level++;
        }
    }
    return {-1, queue<Position>()}; // 如果没有找到路径
}


pair<int, queue<Position>> ShortestPathGetter::shortestPath(Position start, Position &end, char type_dest) {

    if (this->blocked.find(end.get_symbol()) != this->blocked.end()) {
        return {-1, queue<Position>()};
    }
    int rows = 200;
    int cols = 200;

    // 初始化 visited 数组
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    queue<pair<Position, queue<Position>>> q; // 存储 (x, y, path)
    queue<Position> path;
//    path.push(start);
    q.push({start, path});
    visited[start.x][start.y] = true;

    while (!q.empty()) {
        auto front = q.front();
        q.pop();
        Position current = front.first;
        queue<Position> current_path = front.second;

        if (current.get_symbol() == type_dest) {
            end = current;
            return {current_path.size() - 1, current_path}; // 返回路径长度和路径
        }

        // 尝试四个方向移动
        for (auto direction: this->directions) {
            Position attempt = Position(current, direction);

            if (isValidMove(visited, attempt)) {
                queue<Position> new_path = current_path;
                new_path.push(attempt);
                q.push({attempt, new_path});
                visited[attempt.x][attempt.y] = true;
            }
        }
    }

    return {-1, queue<Position>()}; // 如果没有找到路径
}


std::queue<Position> ShortestPathGetter::reconstructPath(APosition* current) {
    std::vector<Position> path;
    while (current) {
        path.push_back(*current);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    // 可以改成栈 但是其他调用代码要重写 暂时不动
    std::queue<Position> res;
    // 不要记录起点
    for (int i = 1; i < path.size(); i++){
        res.push(path[i]);
    }
    return res;
}

void ShortestPathGetter::reconstructPath(APosition* current, bool reverse, queue<Position>& res) {
    std::vector<Position> path;
    while (current) {
        path.push_back(*current);
        current = current->parent;
    }
    if (reverse){
        std::reverse(path.begin(), path.end());
    }
    // 可以改成栈 但是其他调用代码要重写 暂时不动
    while(!res.empty()){
        res.pop();
    }
    // 不要记录起点
    for (int i = 1; i < path.size(); i++){
        res.push(path[i]);
    }

}

//第三个参数暂时还没用上 后面可能会用
pair<int, queue<Position>> ShortestPathGetter::shortestPath(Position start, Position &end, int goods_value = 0, int level=200) {

    if (this->blocked.find(end.get_symbol()) != this->blocked.end()) {
        return {-1, queue<Position>()};
    }
    int rows = 200;
    int cols = 200;

    // 初始化 visited 数组
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    queue<pair<Position, queue<Position>>> q; // 存储 (x, y, path)
    queue<Position> path;
//    path.push(start);
    q.push({start, path});
    visited[start.x][start.y] = true;
    int cur_level = 0;
    Position mark = q.front().first;
    while (cur_level < level && !q.empty()) {
        auto front = q.front();
        q.pop();
        Position current = front.first;
        queue<Position> current_path = front.second;

        if ((base_DS::posi_goods.find(current) != base_DS::posi_goods.end()) &&
            (base_DS::goods[current.x][current.y].remain_frame > current_path.size())) {
            end = current;
            return {current_path.size(), current_path}; // 返回路径长度和路径
        }

        // 尝试四个方向移动
        for (auto direction: this->directions) {
            Position attempt = Position(current, direction);

            if (isValidMove(visited, attempt)) {
                queue<Position> new_path = current_path;
                new_path.push(attempt);
                q.push({attempt, new_path});
                visited[attempt.x][attempt.y] = true;
            }
        }
        if (front.first==mark){
            mark = front.first;
            cur_level++;
        }
    }

    return {-1, queue<Position>()}; // 如果没有找到路径
}

// A*算法
// 0找berth 1找货物 -1找current_dest
pair<int, queue<Position>> ShortestPathGetter::shortestPath(Position start, Position &sub_dest, Position current_dest, int type) {
    int rows = 200;
    int cols = 200;

    // Create a priority queue for open cells
    priority_queue<APosition, vector<APosition>, CompareAPosition> openAPositions;
//    queue<APosition> openAPositions;
    unordered_set<Position, PositionHash> closedPosition;
    // Create a 2D array to store cell information
    vector<vector<APosition>> cells(rows, vector<APosition>(cols));

    // Initialize cells
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cells[i][j].x = i;
            cells[i][j].y = j;
            cells[i][j].f = INT_MAX;
            cells[i][j].g = INT_MAX;
            cells[i][j].h = INT_MAX;
        }
    }

    // Initialize starting cell
    cells[start.x][start.y].f = 0;
    cells[start.x][start.y].g = 0;
    cells[start.x][start.y].h = start-current_dest;

    openAPositions.push(cells[start.x][start.y]);

    while (!openAPositions.empty()) {
        // Get the cell with the lowest f value
        APosition current = openAPositions.top();
//        Position posi_current = Position(current);
        // 后续操作应该会进行自动类型转换
        openAPositions.pop();
//        cout << current;
        closedPosition.insert(current);
        // Check if we reached the goal
        if (find_it(current, type, current_dest, 400)) {
//            cout << "Path found!" << endl;
            sub_dest=current;
            auto res = reconstructPath(&cells[current.x][current.y]);
            int length = res.size()>0?res.size():-1;
            return {length, res};
        }

        // Check all possible movements
        for (int i = 0; i < 4; ++i) {
            Position attempt(current, {directions[i]});
            // Check if the new cell is valid
            if (isValidMove(closedPosition,attempt)) {
                int newG = current.g + 1;
                int newH = attempt-current_dest;
                int newF = newG + newH;

                // Check if the new cell is better than the previous one
                if (cells[attempt.x][attempt.y].f == INT_MAX || newF < cells[attempt.x][attempt.y].f) {
                    cells[attempt.x][attempt.y].parent = &cells[current.x][current.y];
                    cells[attempt.x][attempt.y].f = newF;
                    cells[attempt.x][attempt.y].g = newG;
                    cells[attempt.x][attempt.y].h = newH;
                    openAPositions.push(cells[attempt.x][attempt.y]);
                }
            }
        }
    }

//    cout << "Path not found!" << endl;

    return pair<int, queue<Position>>(-1, queue<Position>());
}

void ShortestPathGetter::get_connected_unsafe_cars(Robot robot, vector<int>& added_cars) {
    int rows = base_DS::N;
    int cols = rows;

    // 初始化 visited 数组
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            visited[i][j] = false;
        }
    }

    queue<Position> q; // 存储 (x, y, path)

    q.push(robot.posi);
    added_cars[robot.robot_id] = robot.robot_id; // 并查集
    visited[robot.posi.x][robot.posi.y] = true;

    while (!q.empty()) {
        auto front = q.front();
        q.pop();
        Position current = front;
        for (auto direction: this->directions) {
            Position attempt = Position(current, direction);
            int robot_id = -1;
            if (isValidMove(visited, attempt, robot_id)) {
                q.push(attempt);
                if (robot_id!=-1){
                    added_cars[robot_id] = robot.robot_id;
                }
                visited[attempt.x][attempt.y] = true;
            }
        }
    }
}
// A.A
bool ShortestPathGetter::isValidMove(bool (*visited)[200], Position posi, int& robot_id) {
    for (int i = 0; i < base_DS::robot_num; i++){
        if (base_DS::robot[i].posi == posi && !visited[posi.x][posi.y]){
            robot_id = base_DS::robot[i].robot_id;
            return true;
        }
        if(!base_DS::robot[i].path.empty()){
            if (base_DS::robot[i].path.front() == posi && !visited[posi.x][posi.y]){
                robot_id = -1; // A.A: .
                return true;
            }
        }
    }
    return false;
}


int test_sp() {
    vector<vector<char>> matrix = {
            {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
            {'#', '.', '.', '.', '#', '.', '.', '.', '.', '#'},
            {'#', '.', '#', '.', '#', '.', '#', '#', '.', '#'},
            {'#', '.', '#', '.', '.', '.', '.', '#', '.', '#'},
            {'#', '.', '#', '#', '#', '#', '#', '#', '.', '#'},
            {'#', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
            {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
    };

    Position start = {1, 1};
    Position end = {5, 8};
    ShortestPathGetter getter;
    auto result = getter.shortestPath(start, end,0,0);
    if (result.first != -1) {
        cout << "最短路径长度为: " << result.first << endl;
        cout << "最短路径为: ";
        queue<Position> shortest_path = result.second;
        while (!shortest_path.empty()) {
            Position point = shortest_path.front();
            cout << "(" << point.x << "," << point.y << ") ";
            shortest_path.pop();
        }
        cout << endl;
    } else {
        cout << "没有找到路径" << endl;
    }

    return 0;
}
