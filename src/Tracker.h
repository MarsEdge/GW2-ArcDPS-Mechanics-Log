#pragma once

#include <mutex>
#include <list>

#include "player.h"
#include "mechanics.h"
#include "npc_ids.h"
#include "helpers.h"

class Tracker
{
public:
	std::mutex players_mtx;
	std::mutex tracker_mtx;
	
	std::list<Player> players;
	
	bool boss_found;
	Boss* boss_data;

	Tracker();
	~Tracker();

	Player* getPlayer(ag* new_player);
	bool addPlayer(char* name, char* account, uintptr_t id);
	bool removePlayer(char* name, char* account, uintptr_t id);
	void addPull(Boss* boss);
	void resetAllPlayerStats();
	uint16_t getMechanicsTotal();

	void processMechanic(Player* new_player_src, Player* new_player_dst, Mechanic* new_mechanic, int64_t value);
};

