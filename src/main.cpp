#include "data_structure.h"
#include <vector>
#include <cstdio>
#include "procedure.h"
#include "update_status.h"
#include "get_shortest_path.h"
#include "get_init_berth_id.h"

#include "dispatch.h"
#include "figure_out.h"
#include "utils_debug.h"
#include "assign_berth.h"
#include <climits>
#include <fstream>
/**
 * 初始化
 */
/*
* 变量名过于普通可能导致冲突，因此使用命名空间包装
*/
//using Direction = std::pair<int, int>;

std::map<Direction ,int>Robot::move_ids = std::map<Direction ,int>();
vector<vector<APosition>> ShortestPathGetter::cells(200, vector<APosition>(200));
bool ShortestPathGetter::visited[200][200];

namespace base_DS {
    int money = 0, boat_capacity = 0, id = 0;
    int inti_length_to_berth = INT_MAX;
    char ch[N][N];
    int gds[N][N];
    bool safe[10];
    bool restoring[10];
    Goods goods[N][N];
    std::unordered_set<Position, PositionHash> posi_goods;
    std::vector<Robot> robot(robot_num);
    std::vector<Berth> berth(berth_num);
    std::vector<Boat> boat(boat_num);
    std::vector<char> symbol_maintained;
    std::vector<Position> posi_symbol;

    int connection[N][N];
    }

namespace flag{
    bool debug = true;
}

void Init()
{
    // Initialize cells
    for (int i = 0; i < 200; ++i) {
        for (int j = 0; j < 200; ++j) {
            ShortestPathGetter::cells[i][j].x = i;
            ShortestPathGetter::cells[i][j].y = j;
            ShortestPathGetter::cells[i][j].parent = nullptr;
        }
    }

    //初始化地图
    //改为从0开始计数
    for(int i = 0; i < base_DS::N; i++){
        scanf("%s", base_DS::ch[i]);
    }

    // 初始化静态方向
    // 0右 1左 2上 3下
    std::vector<Direction> directions = {{0,1},{0,-1},{-1,0},{1,0}};
    for (int i = 0; i < 4; i++){
        Robot::move_ids.insert(std::pair<Direction, int>(directions[i],i));
    }

    //初始化港口
    for(int i = 0; i < base_DS::berth_num; i ++)
    {
        int id;
        scanf("%d", &id);
        int x, y, transport_time, loading_speed;
        scanf("%d%d%d%d", &x, &y,
              &transport_time, &loading_speed);
        base_DS::berth[id] = Berth(x, y, transport_time, loading_speed, id);
    }

    //初始化机器人
    int id_robot = 0;
    for(int i = 0; i < 200; i++){
        for (int j = 0; j < 200; j++){
            if (base_DS::ch[i][j] == 'A'){
                base_DS::robot[id_robot++]=Robot(i, j, id_robot);
            }
        }
    }

    /*初始化整个地图连通性*/
    for (int i = 0; i < base_DS::N; ++i)
        for (int j = 0; j < base_DS::N; ++j)
            base_DS::connection[i][j] = -1;  //初始化为-1
    //根据每个机器人开始bfs，来初始化地图连通性
    for(int i = 0; i < base_DS::robot_num; i ++) {
        int robot_x = base_DS::robot[i].posi.x;
        int robot_y = base_DS::robot[i].posi.y;
        if (base_DS::connection[robot_x][robot_y] == -1) {    //只有未被标记的需要标记连通性
            ShortestPathGetter getter;
            getter.set_connected_set(base_DS::robot[i].posi, i);   //正好以机器人号作为联通标记
        }
    }

    //初始化机器人负责的港口，暂时定一人一个
    int & inti_length_to_berth = base_DS::inti_length_to_berth;
//    vector<int> berth_id = {0,0,1,1,6,6,8,8,9,9}; // map1
//    vector<int> berth_id = {0,1,2,3,4,5,6,7,8,9}; // map-3.11
    vector<int> berth_id = {0,1,2,3,4,5,6,7,8,9}; // map-3.12
//    vector<int> berth_id = {0,0,3,3,3,4,8,8,9,9}; // map-3.9
//    vector<int> berth_id = {2,2,3,3,4,4,6,6,7,7}; // map-3.10

//    vector<int> berth_id = get_init_berth_id(base_DS::berth, base_DS::robot);

    for(int i = 0; i < base_DS::robot_num; i ++) {
//        assign_berth(base_DS::robot[i]);
        base_DS::robot[i]._set_destinations({base_DS::berth[berth_id[i]]});
        if (!base_DS::robot[i].dead && base_DS::robot[i].path.size() < inti_length_to_berth){
            inti_length_to_berth = base_DS::robot[i].path.size();
        }
        base_DS::safe[i] = true;
        base_DS::restoring[i] = false;
    }

    if (inti_length_to_berth==0 || inti_length_to_berth==INT_MAX){
        inti_length_to_berth=20;
    }


    //输入船容量
    scanf("%d", &base_DS::boat_capacity);
    for (int i = 0; i < base_DS::boat_num; i++){
        base_DS::boat[i]=Boat(base_DS::boat_capacity, i);
    }

    //输入结束
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

/**
 * 根据每轮输入更新地图信息
 */
int Input()
{
    //输入帧id，当前获取总价值
    scanf("%d%d", &base_DS::id, &base_DS::money);

    //对之前货物剩余时间-1
    std::unordered_set<Position, PositionHash> pending2delete;
    for (auto posi : base_DS::posi_goods){
        if (0 < base_DS::goods[posi.x][posi.y].remain_frame){
            bool vanish = base_DS::goods[posi.x][posi.y].decrease_remain();
            if (vanish){
                base_DS::goods[posi.x][posi.y].reset(); // 清除货品保存的所有状态
                pending2delete.insert(posi);
            }
        }
    }
    //删除无效货品位置
    for(auto posi:pending2delete){
        base_DS::posi_goods.erase(posi);
    }

    //输入货物信息
    int num;    //货物数量
    scanf("%d", &num);
    for(int i = 1; i <= num; i ++)  //输入货物坐标及价值
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
//        base_DS::gds[x][y] = val; //不使用

        base_DS::goods[x][y].set_value(val, Position(x, y));
        base_DS::posi_goods.insert(Position(x, y));

        /*更新机器人视野内货物*/
        for (auto& robot : base_DS::robot) {
            int robot_x = robot.posi.x;
            int robot_y = robot.posi.y;
            //曼哈顿距离小于视野
            if (abs(x-robot_x) + abs(y-robot_y) <= robot.view)
                robot.posi_goods_in_view.insert(Position(x, y));
        }
        /*更新机器人负责区域内货物*/
    }


    //输入机器人信息
    //他给的机器人的顺序和我在地图上扫描出来的可能不一致 需要解决
    for(int i = 0; i < base_DS::robot_num; i++)
    {
        int sts,goods,x,y;
        scanf("%d%d%d%d", &goods,
              &x, &y, &sts);
        if (base_DS::id == 1){

            // 暂时没考虑要不要深拷贝
            std::vector<Robot> robot_copy = base_DS::robot;
            for (int j = 0; j < base_DS::robot_num; j++){
                if (robot_copy[j].posi==Position(x,y)){
                    base_DS::robot[i] = robot_copy[j];
                    base_DS::robot[i].robot_id = i;
                    break;
                }
            }

        }
        update_robot_status(goods, x, y, sts, base_DS::robot[i]);

        //撞车路径重调度

        //
    }
    //更新港口状态
    update_berth_status();
    //输入船信息
    for(int i = 0; i < 5; i ++){
        int sts, pos;
        scanf("%d%d\n", &sts, &pos);
        update_boat_status(sts, pos, base_DS::boat[i], i);
    }

    //输入结束
    char okk[100];
    scanf("%s", okk);

    return base_DS::id;
}

void allocate_memory(){
//    char data[base_DS::N][base_DS::N];
//    base_DS::ch = *data;
//    base_DS::ch = (char**)malloc(base_DS::N * sizeof(char*));
//    for (int i = 0; i<base_DS::N; i++){
//        base_DS::ch[i] = (char*)malloc(base_DS::N * sizeof(char));
//    }
//    base_DS::gds = (int**)malloc(base_DS::N * sizeof(int*));
//    for (int i = 0; i<base_DS::N; i++){
//        base_DS::gds[i] = (int*)malloc(base_DS::N * sizeof(int));
//    }
}
int main()
{
    allocate_memory();

        Procedure p = Procedure();
        Dispatch d(base_DS::robot_num);
        Figure_out f = Figure_out();
        Init();
        for(int frame = 1; frame <= 15000; frame++)
        {
            int id = Input();
            d.do_dispatch();
            f.do_figure_out();
            p.do_procedure();
            d.restore(); //当前帧即恢复
            puts("OK");
            fflush(stdout);
            if (flag::debug){
                std::ofstream file("money.txt", std::ios::out | std::ios::app);
                file << "id " << base_DS::id << endl;
                file << "current_money " << base_DS::money << endl;
                file.close();

                if (base_DS::id == 15000){
                    std::ofstream file("value_leaf.txt", std::ios::out | std::ios::app);

                    for(int i = 0; i < base_DS::berth_num; i++){
                        int sum = 0;
                        auto copy = base_DS::berth[i].queue_goods_value;
                        file << "berth_id: " << i << endl;
                        file << "loading_speed: " << base_DS::berth[i].loading_speed << endl;
                        file << "transport time: " << base_DS::berth[i].transport_time << endl;
                        file << "num_left: " << copy.size() << endl;
                        while(!copy.empty()){
                            sum += copy.front();
                            copy.pop();
                        }
                        file << "money_left: " << sum << endl;
                    }
                    file.close();
                }
            }
        }

    return 0;
}
