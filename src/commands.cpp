
#include <iostream>
#include <sstream>

#include "data_structure.h"

#include "commands.h"


/**
 * 机器人move指令
 * @param id_robot 机器人id, [0, 9]
 * @param direction 行动方向, 0右 1左 2上 3下
 * @return move id direction 回车
 */
std::string get_MOVE_command(int id_robot, int direction){
    std::stringstream ss;
    ss << "move " << id_robot << " " << direction << std::endl;
    return ss.str();
}
/**
 * 取货get指令
 * @param id_robot 机器人id，取值范围[0, 9]
 * @return get id 回车
 */
std::string get_GET_command(int id_robot){
    std::stringstream ss;
    ss << "get " << id_robot << std::endl;
    return ss.str();
}
/**
 * 放置货物pull指令
 * @param id_robot 机器人id, [0, 9]
 * @return pull id
 */
std::string get_PULL_command(int id_robot){
    std::stringstream ss;
    ss << "pull " << id_robot << std::endl;
    return ss.str();
}

/**
 * ship 船移动到指定泊位
 * @param id_boat 船id, [0, 4]
 * @param id_berth 泊位id, [0, 9]
 * @return ship id_boat id_berth 回车
 */
std::string get_SHIP_command(int id_boat, int id_berth){
    std::stringstream ss;
    ss << "ship " << id_boat << " " << id_berth << std::endl;
    return ss.str();
}

/**
 * go指令 船移动到虚拟点存货
 * @param id_boat 船id, [0, 4]
 * @return
 */
std::string get_GO_command(int id_boat){
    std::stringstream ss;
    ss << "go " << id_boat << std::endl;
    return ss.str();
}