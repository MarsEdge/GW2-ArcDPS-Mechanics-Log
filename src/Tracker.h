#pragma once

#include <mutex>
#include <list>
#include <string>

#include "player.h"
#include "mechanics.h"
#include "npc_ids.h"
#include "helpers.h"
#include "LogEvent.h"

class Tracker
{
public:
	std::mutex players_mtx;
	std::mutex tracker_mtx;
	std::mutex log_events_mtx;
	
	std::list<Player> players;
	
	std::list<LogEvent> log_events;
	int max_log_events = 300;
	
	Boss* boss_data = nullptr;
	int64_t start_time = 0;

	Player* getPlayer(const ag* new_player);
	Player* getPlayer(uintptr_t new_player);
	Player* getPlayer(std::string new_player);
	bool addPlayer(ag* src, ag* dst);//src&dst of combat event
	bool removePlayer(const ag* src);
	Player generatePlayer(char* name, char* account, uintptr_t id);

	void addPull(Boss* boss);
	void resetAllPlayerStats();
	uint16_t getMechanicsTotal();
	uint8_t getPlayerNumInCombat();

	void processCombatEnter(const cbtevent* ev, ag* new_agent);
	void processCombatExit(const cbtevent* ev, ag* new_agent);
	void processMechanic(const cbtevent* ev, Player* new_player_src, Player* new_player_dst, Mechanic* new_mechanic, int64_t value);

	int Tracker::getElapsedTime(uint64_t const &current_time) noexcept;
};

