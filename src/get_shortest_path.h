//
// Created by Administrator on 2024/3/7.
//
#include "Position.h"
#include <vector>
#include<set>
#include "Area.h"
#include <queue>
#include <unordered_set>
#include "Robot.h"

#ifndef LOVEAPPLE_GET_SHORTEST_PATH_H
#define LOVEAPPLE_GET_SHORTEST_PATH_H
using namespace std;

using Path = std::queue<Position>;
class ShortestPathGetter{
public:
    static std::queue<Position> reconstructPath(APosition* current);
    static bool visited[200][200];
    static vector<vector<APosition>> cells;

    bool isValidMove(vector<vector<bool>>& visited, Position posi);
    bool isValidMove(bool visited[200][200], Position posi);
    bool isValidMove(unordered_set<Position, PositionHash>visited, Position posi);
    bool isValidMove(bool visited[200][200], Position posi, int& robot_id);
    bool find_it(Position posi, int type, Position current_dest, int length);
    // 定义上、下、左、右四个方向
    vector<pair<int,int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    std::unordered_set<char> blocked;
    ShortestPathGetter();
    Area space;
//    pair<int, queue<Position>> shortestPath(Position start, Position end, int level);
    // t是传出参数
    pair<int, queue<Position>> shortestPath (Position start, Position& end, char type_dest);
    pair<int, queue<Position>> shortestPath(Position start, Position& end,  int goods_value, int level);
    pair<int, queue<Position>> shortestPath (Position start, Position& sub_dest, Position current_dest, int type);
    pair<int, queue<Position>> shortestPath(Position start, Position& end, int type);
    void set_connected_set(Position posi_robot);
    void set_connected_set(Position posi_robot, int tag);
    void get_connected_unsafe_cars(Robot robot, vector<int>& added_cars);
};



#endif //LOVEAPPLE_GET_SHORTEST_PATH_H
//void foo(){
//    std::vector<std::vector<int>> openList;
//    std::vector<std::vector<int>> closedList;
//    std::vector<Node> nodes;
//    nodes.push_back(Node(start.first, start.second, 0, heuristic(start, end), nullptr));
//    openList.push_back({start.first, start.second});
//    while (!nodes.empty()) {
//        Node current = nodes.front();
//        nodes.erase(nodes.begin());
//        openList.erase(std::find(openList.begin(), openList.end(), {current.x, current.y}));
//        closedList.push_back({current.x, current.y});
//        if (current.x == end.first && current.y == end.second) {
//            return reconstructPath(&current);
//        }
//        std::vector<std::vector<int>> neighbors;
//        std::vector<int> dx = {-1, 0, 1, 0};
//        std::vector<int> dy = {0, 1, 0, -1};
//        for (int i = 0; i < 4; ++i) {
//            int nx = current.x + dx[i];
//            int ny = current.y + dy[i];
//            if (isValid(nx, ny, grid)) {
//                neighbors.push_back({nx, ny});
//            }
//        }
//        for (const std::vector<int>& neighbor : neighbors) {
//            int tentativeCost = current.cost + 1;
//            Node& neighborNode = nodes[neighbor[0] - start.first][neighbor[1] - start.second];
//            if (std::find(closedList.begin(), closedList.end(), neighbor) != closedList.end()) {
//                continue;
//            }
//            if (tentativeCost < neighborNode.cost || std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
//                neighborNode.parent = &current;
//                neighborNode.cost = tentativeCost;
//                neighborNode.heuristic = heuristic(neighbor, end);
//                if (std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
//                    openList.push_back(neighbor);
//                }
//            }
//        }
//    }
//
//}