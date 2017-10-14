#pragma once

#include <stdint.h>
#include "arcdps_datastructures.h"

#define MAX_PLAYER_COUNT 10

struct Player
{
    uint16_t id;            //instance id
    int downs;              //number of times the player has downed
    bool is_downed;     //is currently is down state
    int mechanics_failed;   //number of mechanics failed
    uint64_t last_stab_time;  //time stability is going to expire
    uint64_t last_hit_time;       //time player was last hit with a mechanic
    uint16_t last_machanic;       //skill id of last failed mechanic

    Player();

    Player(uint16_t new_id);
    void reset_stats();
    void reset_all();
};

extern Player players[MAX_PLAYER_COUNT];

Player* get_player(uint16_t new_id);
bool is_player(ag* new_player);
void reset_all_player_stats();
