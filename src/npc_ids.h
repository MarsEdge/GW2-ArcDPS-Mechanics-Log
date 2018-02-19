#pragma once

#include <stdint.h>
#include <vector>
#include <string>

const uint32_t ID_vale_guardian  = 0x3C4E;
const uint32_t ID_gorseval       = 0x3C45;
const uint32_t ID_sabetha        = 0x3C0F;
const uint32_t ID_slothasor      = 0x3EFB;
const uint32_t ID_trio_burg      = 0x3ED8;
const uint32_t ID_trio_zane      = 0x3F09;
const uint32_t ID_trio_narella   = 0x3EFD;
const uint32_t ID_matthias       = 0x3EF3;
const uint32_t ID_keep_construct = 0x3F6B;
const uint32_t ID_xera_part1     = 0x3F76;
const uint32_t ID_xera_part2     = 0x3F9E;
const uint32_t ID_cairn          = 0x432A;
const uint32_t ID_overseer       = 0x4314;
const uint32_t ID_samarog        = 0x4324;
const uint32_t ID_deimos         = 0x4302;
const uint32_t ID_soulless_horror= 0x4D37;
const uint32_t ID_dhuum          = 0x4BFA;

struct boss
{
    std::vector<uint32_t> ids;//npc id (from ag->prof)
    std::string name;
    int64_t timer;//enrage timer
    uint64_t health;

    boss();

    boss set_ids(std::initializer_list<uint32_t> const new_ids) {this->ids = std::vector<uint32_t>(new_ids); return *this;}
    boss set_name(std::string const new_name) {this->name = new_name; return *this;}
    boss set_timer(uint64_t const new_timer) {this->timer = new_timer; return *this;}
    boss set_health(uint64_t const new_health) {this->health = new_health; return *this;}
};

extern std::vector<boss> bosses;
