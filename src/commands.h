
#include <string>

#ifndef C_COMMANDS_H
#define C_COMMANDS_H

std::string get_MOVE_command(int id_robot, int direction);
std::string get_GET_command(int id_robot);
std::string get_PULL_command(int id_robot);
std::string get_SHIP_command(int id_boat, int id_berth);
std::string get_GO_command(int id_boat);

#endif //C_COMAENDS_H
