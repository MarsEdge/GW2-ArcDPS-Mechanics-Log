#include <stdint.h>

typedef struct Player
{
    uint16_t id = 0;            //instance id
    int downs = 0;              //number of times the player has downed
    bool is_downed = false;     //is currently is down state
    int mechanics_failed = 0;   //number of mechanics failed
    uint64_t last_stab_time = 0;  //time stability is going to expire
    uint64_t last_hit_time=0;       //time player was last hit with a mechanic
    uint16_t last_machanic=0;       //skill id of last failed mechanic

    Player(uint16_t new_id)
    {
        id = new_id;
    }
    void reset_stats()
    {
        downs = 0;
        is_downed = false;
        mechanics_failed = 0;
        last_stab_time = 0;
    }
};
