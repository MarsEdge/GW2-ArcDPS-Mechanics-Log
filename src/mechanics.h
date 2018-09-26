#pragma once

#include <string>
#include <vector>

#include "arcdps_datastructures.h"
#include "player.h"
#include "skill_ids.h"
#include "npc_ids.h"
#include "helpers.h"

extern bool have_added_line_break;
extern uint64_t last_mechanic_time;
extern uint64_t line_break_frequency;
extern bool has_logged_mechanic;

const unsigned int ms_per_tick = 40;// 1000/25
const unsigned int combatapi_delay = 5000;

enum Verbosity
{
	verbosity_chart = 1 << 0,
	verbosity_log = 1 << 1
};

struct Mechanic
{
    std::string name; //name of mechanic
    std::vector<uint32_t> ids; //skill ids;
    Boss* boss;//required boss, ignored if null
    uint64_t frequency_player; //minimum time between instances of this mechanic per player(ms)
    uint64_t frequency_global; //minimum time between instances of this mechanic globally(ms)
    uint64_t last_hit_time; //time of last instance of mechanic
	uint8_t is_activation;
	uint8_t is_buffremove;
	int32_t overstack_value;//required overstack value
	int32_t value;//required value
    bool is_interupt;
    bool is_multihit;
    bool target_is_dst;
    bool fail_if_hit;
    bool valid_if_down; //mechanic counts if player is in down-state

	bool can_evade;
	bool can_block;
	bool can_invuln;

	int verbosity;

    Mechanic();

	int64_t isValidHit(cbtevent* ev, Player* src, Player* dst);

	std::string getIniName();

    bool (*special_requirement)(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player);
    int64_t (*special_value)(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player);

    Mechanic setName(std::string const new_name) {this->name = new_name; return *this;}
    Mechanic setIds(std::initializer_list<uint32_t> const new_ids) {this->ids = std::vector<uint32_t>(new_ids); return *this;}
    Mechanic setBoss(Boss* const new_boss) {this->boss = new_boss; return *this;}
    Mechanic setFrequencyPlayer(uint64_t const new_frequency_player) {this->frequency_player = new_frequency_player; return *this;}
    Mechanic setFrequencyGlobal(uint64_t const new_frequency_global) {this->frequency_global = new_frequency_global; return *this;}
    Mechanic setIsActivation(uint8_t const new_is_activation) {this->is_activation = new_is_activation; return *this;}
    Mechanic setIsBuffremove(uint8_t const new_is_buffremove) {this->is_buffremove = new_is_buffremove; return *this;}
	Mechanic setOverstackValue(int32_t const new_overstack_value) { this->overstack_value = new_overstack_value; return *this; }
	Mechanic setValue(int32_t const new_value) { this->value = new_value; return *this; }
    Mechanic setIsInterupt(bool const new_is_interupt) {this->is_interupt = new_is_interupt; return *this;}
    Mechanic setIsMultihit(bool const new_is_multihit) {this->is_multihit = new_is_multihit; return *this;}
    Mechanic setTargetIsDst(bool const new_target_is_dst) {this->target_is_dst = new_target_is_dst; return *this;}
    Mechanic setFailIfHit(bool const new_fail_if_hit) {this->fail_if_hit = new_fail_if_hit; return *this;}
    Mechanic setValidIfDown(bool const new_valid_if_down) {this->valid_if_down = new_valid_if_down; return *this;}
	Mechanic setCanEvade(bool const new_can_evade) { this->can_evade = new_can_evade; return *this; }
	Mechanic setCanBlock(bool const new_can_block) { this->can_block = new_can_block; return *this; }
	Mechanic setCanInvuln(bool const new_can_invuln) { this->can_invuln = new_can_invuln; return *this; }
	Mechanic setVerbosity(int const new_verbosity) { this->verbosity = new_verbosity; return *this; }

    Mechanic setSpecialRequirement(bool (*new_special_requirement)(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player)) {this->special_requirement = new_special_requirement; return *this;}
    Mechanic setSpecialReturnValue(int64_t(*new_special_value)(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player)) {this->special_value = new_special_value; return *this;}
};

bool requirementDefault(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player);
bool requirementConjure(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player);
bool requirementDhuumSnatch(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player);

int64_t valueDefault(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player);
int64_t valueDhuumShackles(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player);

const uint16_t max_deimos_oils = 3;
struct DeimosOil
{
	uint16_t id = 0;
	uint64_t first_touch_time = 0;
	uint64_t last_touch_time = 0;
};

extern DeimosOil deimos_oils[max_deimos_oils];

bool requirementDeimosOil(const Mechanic & current_mechanic, cbtevent * ev, Player * src, Player * dst, Player * current_player);

bool requirementOnSelf(const Mechanic & current_mechanic, cbtevent * ev, Player * src, Player * dst, Player * current_player);


extern std::vector <Mechanic> mechanics;
