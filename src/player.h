#pragma once

#include <stdint.h>
#include <string>
#include <cstring>
#include <vector>
#include <mutex>
#include "arcdps_datastructures.h"

#define MAX_PLAYER_COUNT 10

struct Player
{
    std::string name;
    uintptr_t id;            //instance id
    uint16_t downs;              //number of times the player has downed
    uint16_t deaths;              //number of times the player has completely died
    bool is_downed;     //is currently is down state
    uint16_t mechanics_failed;   //number of mechanics failed
    uint16_t mechanics_received;//number of boss mechanics received
    uint64_t last_stab_time;  //time stability is going to expire
    uint64_t last_hit_time;       //time player was last hit with a mechanic
    uint16_t last_mechanic;       //skill id of last failed mechanic

    Player();
    Player(ag* new_player);

    void down();
    void dead();
    void rally();

    void mechanic_fail();
    void mechanic_receive();
    bool is_relevant();     //if player is relevant for displaying

    uint64_t get_last_stab_time();
    void set_stab_time(uint64_t new_stab_time);

    uint64_t get_last_hit_time();
    void set_last_hit_time(uint64_t new_hit_time);

    uint16_t get_last_mechanic();
    void set_last_mechanic(uint16_t new_mechanic);
};

extern std::mutex players_mtx;

extern std::vector<Player> players;

Player* get_player(ag* new_player);
bool is_player(ag* new_player);
void reset_all_player_stats();
