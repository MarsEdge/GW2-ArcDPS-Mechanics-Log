#pragma once

#include <stdint.h>
#include <malloc.h>
#include <cstdlib>
#include "arcdps_datastructures.h"
#include "npc_ids.h"

int split (const char *str, char c, char ***arr);

struct game_state
{
    ag* current_boss;
    boss boss_data;
};
