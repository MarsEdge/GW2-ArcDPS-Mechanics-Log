#include "Tracker.h"

Player* Tracker::getPlayer(const ag* new_player)
{
	if (!isPlayer(new_player)) return nullptr;
	auto it = std::find(players.begin(), players.end(), new_player->id);

	//player not tracked yet
	if (it == players.end())
	{
		return nullptr;
	}
	else//player tracked
	{
		return &*it;
	}
}

Player * Tracker::getPlayer(uintptr_t new_player)
{
	if (!new_player) return nullptr;

	auto it = std::find(players.begin(), players.end(), new_player);

	//player not tracked yet
	if (it == players.end())
	{
		return nullptr;
	}
	else//player tracked
	{
		return &*it;
	}
}

Player * Tracker::getPlayer(std::string new_player)
{
	if (new_player.empty()) return nullptr;
	auto it = std::find(players.begin(), players.end(), new_player);

	//player not tracked yet
	if (it == players.end())
	{
		return nullptr;
	}
	else//player tracked
	{
		return &*it;
	}
}

bool Tracker::addPlayer(ag* src, ag* dst)
{
	if (!src) return false;
	if (!dst) return false;

	char* name = src->name;
	char* account = dst->name;
	const uintptr_t id = src->id;

	if (!name) return false;
	if (!account) return false;
	if (std::string(name).length() < 2) return false;
	if (std::string(account).length() < 2) return false;

	Player* new_player = getPlayer(account);
	//player not tracked yet
	if (!new_player)
	{
		players.push_back(generatePlayer(name, account, id));
	}
	else//player tracked
	{
		new_player->id = id;
		new_player->name = name;
		new_player->in_squad = true;
	}
	return true;
}

bool Tracker::removePlayer(const ag* src)
{
	if (!src) return false;

	const char* name = src->name;
	const char* account = src->name;//TODO: if account names are ever added, put it here
	const uintptr_t id = src->id;

	Player* new_player = getPlayer(id);

	//player not tracked yet
	if (!new_player)
	{
		return false;
	}
	else
	{
		new_player->in_squad = false;
		return true;
	}
}

Player Tracker::generatePlayer(char* name, char* account, uintptr_t id)
{
	Player out = Player(name, account, id);

	for (auto mechanic = mechanics.begin(); mechanic != mechanics.end(); ++mechanic)
	{
		out.receiveMechanic(0,mechanic->name, mechanic->ids[0], mechanic->fail_if_hit, mechanic->boss);
	}

	out.resetStats();

	return out;
}

void Tracker::addPull(Boss* boss)
{
	if (!boss) return;
	
	boss->pulls++;

	for (auto player = players.begin(); player != players.end(); ++player)
	{
		player->addPull(boss);
	}
}

void Tracker::resetAllPlayerStats()
{
	std::lock_guard<std::mutex> lg(players_mtx);
	for (auto player = players.begin(); player != players.end(); ++player)
	{
		player->resetStats();
	}
}

uint16_t Tracker::getMechanicsTotal()
{
	uint16_t result = 0;

	for (auto player = players.begin(); player != players.end(); ++player)
	{
		if (player->isRelevant())
		{
			result += player->getMechanicsTotal();
		}
	}
	return result;
}

uint8_t Tracker::getPlayerNumInCombat()
{
	uint8_t result = 0;

	for (auto player = players.begin(); player != players.end(); ++player)
	{
		if (player->in_squad && player->in_combat)
		{
			result++;
		}
	}
	return result;
}

void Tracker::processCombatEnter(const cbtevent* ev, ag* new_agent)
{
	Player* new_player = nullptr;
	if (new_player = getPlayer(new_agent))
	{
		new_player->combatEnter();

		if (new_agent && new_agent->self)
		{
			start_time = ev->time;

			if (has_logged_mechanic)
			{
				has_logged_mechanic = false;
				log_events.push_back(LogEvent(nullptr, nullptr, getElapsedTime(ev->time), 1));//TODO: make function for pushing log events

				if (log_events.size() > max_log_events)
				{
					log_events.pop_front();
				}
			}
		}
	}

	if (!boss_data && new_agent)
	{
		for (auto current_boss = bosses.begin(); current_boss != bosses.end(); ++current_boss)
		{
			if ((*current_boss)->hasId(new_agent->prof))
			{
				boss_data = *current_boss;
			}
		}
	}
}

void Tracker::processCombatExit(const cbtevent* ev, ag* new_agent)
{
	Player* new_player = nullptr;
	if (new_player = getPlayer(new_agent))
	{
		new_player->combatExit();

		if (getPlayerNumInCombat() == 0)
		{
			addPull(boss_data);
			boss_data = nullptr;
		}
	}
}

void Tracker::processMechanic(const cbtevent* ev, Player* new_player_src, Player* new_player_dst, Mechanic* new_mechanic, int64_t value)
{
	std::lock_guard<std::mutex> lg(tracker_mtx);

	Player* relevant_player = new_mechanic->target_is_dst ? new_player_dst : new_player_src;

	if (!relevant_player) return;

	if (new_mechanic->is_multihit && ev->time < (relevant_player->getLastMechanicHitTime(new_mechanic->ids[0]) + new_mechanic->frequency_player)) return;//check mechanic timing again to prevent race conditions

	if (new_mechanic->verbosity & verbosity_log)
	{
		std::lock_guard<std::mutex> lg2(log_events_mtx);
		
		log_events.push_back(LogEvent(relevant_player, new_mechanic, getElapsedTime(ev->time), value));
		
		if (log_events.size() > max_log_events)
		{
			log_events.pop_front();
		}
		has_logged_mechanic = true;
	}
	
	if (new_mechanic->verbosity & verbosity_chart)
	{
		relevant_player->receiveMechanic(ev->time, new_mechanic->name, new_mechanic->ids[0], new_mechanic->fail_if_hit, new_mechanic->boss);
	}
}

int Tracker::getElapsedTime(uint64_t const &current_time) noexcept
{
	return (current_time - start_time) / 1000;
}