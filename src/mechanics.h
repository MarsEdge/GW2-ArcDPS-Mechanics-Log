#pragma once

#include <string>
#include <vector>

#include "arcdps_datastructures.h"
#include "player.h"
#include "skill_ids.h"
#include "npc_ids.h"
#include "helpers.h"
#include "Tracker.h"

extern bool have_added_line_break;
extern uint64_t last_mechanic_time;
extern uint64_t line_break_frequency;
extern bool has_logged_mechanic;

const unsigned int ms_per_tick = 40;// 1000/25
const unsigned int combatapi_delay = 5000;

struct mechanic
{
    std::string name; //name of mechanic
    std::vector<uint16_t> ids; //skill ids;
    uint32_t boss_id;//required boss id, ignored if 0
    uint64_t frequency_player; //minimum time between instances of this mechanic per player(ms)
    uint64_t frequency_global; //minimum time between instances of this mechanic globally(ms)
    uint64_t last_hit_time; //time of last instance of mechanic
	uint8_t is_activation;
	uint8_t is_buffremove;
	int32_t overstack_value;//required overstack value
    bool is_interupt;
    bool is_multihit;
    bool target_is_dst;
    bool fail_if_hit;
    bool valid_if_down; //mechanic counts if player is in down-state

	bool can_evade;
	bool can_block;
	bool can_invuln;

	uint16_t verbosity;

    mechanic();

    float isValidHit(Tracker* tracker, cbtevent* ev, ag* src, ag* dst);
    bool (*special_requirement)(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player);
    float (*special_value)(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player);

    mechanic setName(std::string const new_name) {this->name = new_name; return *this;}
    mechanic setIds(std::initializer_list<uint16_t> const new_ids) {this->ids = std::vector<uint16_t>(new_ids); return *this;}
    mechanic setBossId(uint32_t const new_boss_id) {this->boss_id = new_boss_id; return *this;}//TODO:use Boss object
    mechanic setFrequencyPlayer(uint64_t const new_frequency_player) {this->frequency_player = new_frequency_player; return *this;}
    mechanic setFrequencyGlobal(uint64_t const new_frequency_global) {this->frequency_global = new_frequency_global; return *this;}
    mechanic setIsActivation(uint8_t const new_is_activation) {this->is_activation = new_is_activation; return *this;}
    mechanic setIsBuffremove(uint8_t const new_is_buffremove) {this->is_buffremove = new_is_buffremove; return *this;}
	mechanic setOverstackValue(int32_t const new_overstack_value) { this->overstack_value = new_overstack_value; return *this; }
    mechanic setIsInterupt(bool const new_is_interupt) {this->is_interupt = new_is_interupt; return *this;}
    mechanic setIsMultihit(bool const new_is_multihit) {this->is_multihit = new_is_multihit; return *this;}
    mechanic setTargetIsDst(bool const new_target_is_dst) {this->target_is_dst = new_target_is_dst; return *this;}
    mechanic setFailIfHit(bool const new_fail_if_hit) {this->fail_if_hit = new_fail_if_hit; return *this;}
    mechanic setValidIfDown(bool const new_valid_if_down) {this->valid_if_down = new_valid_if_down; return *this;}
	mechanic setCanEvade(bool const new_can_evade) { this->can_evade = new_can_evade; return *this; }
	mechanic setCanBlock(bool const new_can_block) { this->can_block = new_can_block; return *this; }
	mechanic setCanInvuln(bool const new_can_invuln) { this->can_invuln = new_can_invuln; return *this; }
	mechanic setVerbosity(uint16_t const new_verbosity) { this->verbosity = new_verbosity; return *this; }

    mechanic setSpecialRequirement(bool (*new_special_requirement)(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)) {this->special_requirement = new_special_requirement; return *this;}
    mechanic setSpecialValue(float (*new_special_value)(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)) {this->special_value = new_special_value; return *this;}
};

bool requirementDefault(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player);
bool requirementConjure(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player);
bool requirementDhuumSnatch(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player);

float valueDefault(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player);
float valueDhuumShackles(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player);

const uint16_t max_deimos_oils = 3;
struct DeimosOil
{
	uintptr_t id = 0;
	uint64_t last_touch_time = 0;
};

extern DeimosOil deimos_oils[max_deimos_oils];

bool requirementDeimosOil(const mechanic & current_mechanic, cbtevent * ev, ag * src, ag * dst, Player * current_player);

bool requirementOnSelf(const mechanic & current_mechanic, cbtevent * ev, ag * src, ag * dst, Player * current_player);


extern std::vector <mechanic> mechanics;
