#include "Tracker.h"



Tracker::Tracker()
{
}


Tracker::~Tracker()
{
}

Player* Tracker::getPlayer(ag* new_player)
{
	if (!isPlayer(new_player)) return nullptr;
	std::lock_guard<std::mutex> lock(players_mtx);
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

bool Tracker::addPlayer(char* name, char* account, uintptr_t id)
{
	if (!name) return false;
	if (!account) return false;

	std::lock_guard<std::mutex> lg(players_mtx);
	auto it = std::find(players.begin(), players.end(), std::string(account));//TODO: something more porformant than makeing a string for comparison?

	//player not tracked yet
	if (it == players.end())
	{
		players.push_back(generatePlayer(name, account, id));
	}
	else//player tracked
	{
		it->id = id;
		it->name = name;
		it->in_squad = true;
	}
	return true;
}

bool Tracker::removePlayer(char* name, char* account, uintptr_t id)
{
	std::lock_guard<std::mutex> lock(players_mtx);
	auto it = std::find(players.begin(), players.end(), id);

	//player not tracked yet
	if (it == players.end())
	{
		return false;
	}
	else
	{
		it->in_squad = false;
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

void Tracker::processCombatEnter(ag* new_agent)
{
	Player* new_player = nullptr;
	if (new_player = getPlayer(new_agent))
	{
		new_player->combatEnter();
	}

	if (!boss_data)
	{
		for (auto current_boss = bosses.begin(); current_boss != bosses.end(); ++current_boss)
		{
			if ((*current_boss)->hasId(new_agent->prof))
			{
				boss_data = *current_boss;
				addPull(boss_data);
			}
		}
	}
}

void Tracker::processCombatExit(ag* new_agent)
{
	Player* new_player = nullptr;
	if (new_player = getPlayer(new_agent))
	{
		new_player->combatExit();
	}

	if (getPlayerNumInCombat() == 0)
	{
		boss_data = nullptr;
	}
}

void Tracker::processMechanic(cbtevent* ev, Player* new_player_src, Player* new_player_dst, Mechanic* new_mechanic, int64_t value)
{
	if (new_mechanic->target_is_dst)
	{
		new_player_dst->receiveMechanic(ev->time, new_mechanic->name, new_mechanic->ids[0], new_mechanic->fail_if_hit, new_mechanic->boss);
	}
	else
	{
		new_player_src->receiveMechanic(ev->time, new_mechanic->name, new_mechanic->ids[0], new_mechanic->fail_if_hit, new_mechanic->boss);
	}
}
