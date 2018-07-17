#pragma once

#include <stdint.h>
#include <vector>
#include <list>
#include <string>

class Boss
{
public:
	std::vector<uint32_t> ids;//npc id (from ag->prof)
	std::string name;
	int64_t timer;//enrage timer
	uint64_t health;
	uint16_t pulls;

	Boss();

	bool hasId(uint32_t new_id);

	Boss setIds(std::initializer_list<uint32_t> const new_ids) { this->ids = std::vector<uint32_t>(new_ids); return *this; }
	Boss setName(std::string const new_name) { this->name = new_name; return *this; }
	Boss setTimer(int64_t const new_timer) { this->timer = new_timer; return *this; }
	Boss setHealth(uint64_t const new_health) { this->health = new_health; return *this; }
};

extern Boss boss_vg;

extern Boss boss_gors;

extern Boss boss_sab;

extern Boss boss_sloth;

extern Boss boss_trio;

extern Boss boss_matti;


//TODO: Add escort boss

extern Boss boss_kc;

extern Boss boss_xera;


extern Boss boss_cairn;

extern Boss boss_mo;

extern Boss boss_sam;

extern Boss boss_deimos;


extern Boss boss_sh;

//TODO: Add soul river boss

extern Boss boss_soul_eater;

extern Boss boss_ice_king;

extern Boss boss_cave;

extern Boss boss_dhuum;


extern Boss boss_mama;

extern Boss boss_siax;

extern Boss boss_ensolyss;


extern Boss boss_skorvald;

extern Boss boss_artsariiv;

extern Boss boss_arkk;

extern std::list<Boss*> bosses;
