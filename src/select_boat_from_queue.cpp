//
// Created by 梅佳伟 on 2024/3/8.
//

#include <climits>
#include "select_boat_from_queue.h"
#include "Boat.h"

int select_boat_from_queue (vector<Boat> boats){
    /*码头的选船策略，遵循贪心原则，选择这些候选船中容量最小的船进行第一个装货
        这是因为使用小的船可以在相同的货物条件下更早产生收益
    */
   if(boats.size() == 0){
    return -1;
   }
   int max_volume = INT_MAX;
   int best_boat_index = 0;
   for(int i = 0; i < boats.size(); i++){
        if(boats[i].capacity < max_volume){
            max_volume = boats[i].capacity;
            best_boat_index = i;
        }
   }
   return best_boat_index;
}
