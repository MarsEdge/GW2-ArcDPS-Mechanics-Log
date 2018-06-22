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
	if (!name || !account)
	{
		return false;
	}

	std::lock_guard<std::mutex> lg(players_mtx);
	auto it = std::find(players.begin(), players.end(), std::string(account));//TODO: something more porformant that makeing a string for comparison?

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