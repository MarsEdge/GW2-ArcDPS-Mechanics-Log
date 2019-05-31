#pragma once

#include <stdint.h>
#include <malloc.h>
#include <cstdlib>
#include "arcdps_datastructures.h"
#include "npc_ids.h"

#define __VERSION__ (__DATE__)

bool isPlayer(const ag* new_player) noexcept;
