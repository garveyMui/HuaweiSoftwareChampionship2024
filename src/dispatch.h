//
// Created by 梅佳伟 on 2024/3/12.
//

#ifndef MAIN_DISPATCH_H
#define MAIN_DISPATCH_H


#include "Position.h"
#include <vector>
#include "Robot.h"

class Dispatch{
public:
    Dispatch(int num_robot);
    void do_dispatch();
    static bool will_collide_eachOther(const Robot& a, const Robot& b);
    static bool wanna_exchange(Robot& a, Robot& b);
    void restore();
    void restore_safe();
    void modify_map();
    void modify_safe();
    char wall = '#';
};


#endif //MAIN_DISPATCH_H
