#pragma once

#include <stdint.h>
#include <string>
#include <cstring>
#include <vector>
#include <mutex>
#include "arcdps_datastructures.h"
#include "npc_ids.h"

struct Player
{
    std::string name = "";
    std::string account = "";
    uintptr_t id = 0;            //instance id
    bool is_downed = false;     //is currently is down state
    bool in_squad = true;          //currently in squad
	bool in_combat = false;
	uint64_t last_stab_time = 0;  //time stability is going to expire

    Player(ag* new_player);
    Player(char* new_name, char* new_account, uintptr_t new_id);

	bool operator==(uintptr_t other_id);
	bool operator==(std::string other_str);
};
