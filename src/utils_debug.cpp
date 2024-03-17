//
// Created by 梅佳伟 on 2024/3/12.
//

#include "utils_debug.h"
std::string vectorPairToString(std::queue<Position> pairs) {
    std::string result;
    while (!pairs.empty()) {
        Position p = pairs.front();
        pairs.pop();
        if (!result.empty()) {
            result += "->";
        }
        result += "( " + std::to_string(p.x) + ", " + std::to_string(p.y)+" )";
    }
    return result;
}