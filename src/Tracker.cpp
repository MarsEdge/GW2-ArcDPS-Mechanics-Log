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
		players.push_back(Player(name, account, id));
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

void Tracker::addPull(Boss* boss)
{
	boss->pulls++;
	for (std::list<Player>::iterator player = players.begin(); player != players.end(); ++player)
	{
		player->addPull(boss);
	}
}

void Tracker::resetAllPlayerStats()
{
	std::lock_guard<std::mutex> lg(players_mtx);
	for (std::list<Player>::iterator player = players.begin(); player != players.end(); ++player)
	{
		player->resetStats();
	}
}

uint16_t Tracker::getMechanicsTotal()
{
	uint16_t result = 0;

	for (std::list<Player>::iterator player = players.begin(); player != players.end(); ++player)
	{
		if (player->isRelevant())
		{
			result += player->getMechanicsTotal();
		}
	}
	return result;
}

void Tracker::processMechanic(Player* new_player_src, Player* new_player_dst, Mechanic* new_mechanic, int64_t value)
{
	if (new_mechanic->target_is_dst)
	{
		new_player_dst->receiveMechanic(new_mechanic->name, new_mechanic->ids[0], new_mechanic->fail_if_hit, new_mechanic->boss);
	}
	else
	{
		new_player_src->receiveMechanic(new_mechanic->name, new_mechanic->ids[0], new_mechanic->fail_if_hit, new_mechanic->boss);
	}
}
