#include "player.h"

std::vector<Player> players;
std::mutex players_mtx;
std::mutex tracker_mtx;

Player::Player()
{
    name = "";
    id = 0;            //instance id
    downs = 0;              //number of times the player has downed
    deaths = 0;
    pulls = 0;
    is_downed = false;     //is currently is down state
    mechanics_failed = 0;   //number of mechanics failed
    mechanics_received = 0;
    last_stab_time = 0;  //time stability is going to expire
    last_hit_time = 0;       //time player was last hit with a mechanic
    last_mechanic = 0;       //skill id of last failed mechanic
    in_squad = true;
	in_combat = false;
}

Player::Player(ag* new_player)
{
    if(new_player->name)
    {
        name = new_player->name;
    }
    else
    {
        name="";
    }
    id = new_player->id;
    downs = 0;              //number of times the player has downed
    deaths = 0;
    pulls = 0;
    is_downed = false;     //is currently is down state
    mechanics_failed = 0;   //number of mechanics failed
    mechanics_received = 0;
    last_stab_time = 0;  //time stability is going to expire
    last_hit_time=0;       //time player was last hit with a mechanic
    last_mechanic=0;       //skill id of last failed mechanic
    in_squad = true;
}

Player::Player(char* new_name, char* new_account, uintptr_t new_id)
{
    name = new_name ? new_name : "";
    account = new_account ? new_account : "";
    id = new_id;
    downs = 0;              //number of times the player has downed
    deaths = 0;
    pulls = 0;
    is_downed = false;     //is currently is down state
    mechanics_failed = 0;   //number of mechanics failed
    mechanics_received = 0;
    last_stab_time = 0;  //time stability is going to expire
    last_hit_time=0;       //time player was last hit with a mechanic
    last_mechanic=0;       //skill id of last failed mechanic
    in_squad = true;
}

bool Player::operator==(uintptr_t other_id)
{
	return id == other_id;
}

bool Player::operator==(std::string other_str)
{
	return name == other_str || account == other_str;
}

Player::MechanicTracker::MechanicTracker(uint64_t new_time, std::string &new_name, uint32_t &new_id,bool &new_fail, Boss* new_boss)
{
    name = new_name;
    id = new_id;
    fail = new_fail;
    current_boss = new_boss;
    hits = 1;
    pulls = new_boss ? new_boss->pulls : 1;
	last_hit_time = new_time;
}

void Player::down()
{
    downs++;
    is_downed = true;
}

void Player::dead()
{
    deaths++;
}

void Player::rally()
{
    is_downed = false;
}

void Player::fixDoubleDown()
{
    if(downs > 0) downs--;
}

void Player::receiveMechanic(uint64_t time, std::string &name, uint32_t &id,bool &is_fail, Boss* boss)
{
    if(!is_fail)
    {
        mechanics_received++;
    }
    else
    {
        mechanics_failed++;
    }
    setLastMechanic(id);
	setLastHitTime(time);
    for(uint16_t index=0;index<tracker.size();index++)
    {
        if(tracker.at(index).id == id)
        {
            tracker.at(index).hits++;
			
			if (tracker.at(index).last_hit_time < time) tracker.at(index).last_hit_time = time;
            return;
        }
    }
    std::lock_guard<std::mutex> lg(tracker_mtx);
    tracker.push_back(MechanicTracker(time,name,id,is_fail,boss));
    return;
}

bool Player::isRelevant()
{
    return downs > 0
    || deaths > 0
    || mechanics_failed > 0
    || mechanics_received > 0
	|| in_squad;

}

uint64_t Player::getLastStabTime()
{
    return last_stab_time;
}

void Player::setStabTime(uint64_t new_stab_time)
{
    if(last_stab_time < new_stab_time)
    {
        last_stab_time = new_stab_time;
    }
}

uint64_t Player::getLastHitTime()
{
    return last_hit_time;
}

void Player::setLastHitTime(uint64_t new_hit_time)
{
    last_hit_time = new_hit_time;
}

uint16_t Player::getLastMechanic()
{
    return last_mechanic;
}

void Player::setLastMechanic(uint32_t new_mechanic)
{
    last_mechanic = new_mechanic;
}

uint64_t Player::getLastMechanicHitTime(uint32_t new_mechanic)
{
	uint64_t out = 0;

	for (uint16_t index = 0; index<tracker.size(); index++)
	{
		if (tracker.at(index).id == new_mechanic)
		{
			out = tracker.at(index).last_hit_time;
			break;
		}
	}
	return out;
}

std::string Player::MechanicTracker::toString()
{
	if (!isRelevant()) return "";

    return 
	(current_boss ? current_boss->name : "") + "," +
	name + "," +
    (!fail ? std::to_string(hits) + "," : "," + std::to_string(hits)) +
    "," + "," + "," +
    std::to_string(pulls) +
    "\n";
}

std::string Player::toString()
{
    std::string output = "";

    output += name + "," +
    account + "," +
	"All" + "," +
    "Overall" + "," +
    std::to_string(mechanics_received) + "," +
    std::to_string(mechanics_failed) + "," +
    std::to_string(downs) + "," +
    std::to_string(deaths) + "," +
    std::to_string(pulls) +
    "\n";

    for(uint16_t index=0; index<tracker.size();index++)
    {
		if (!tracker.at(index).isRelevant()) continue;
		output += name + "," + account + "," +
        tracker.at(index).toString();
    }
    return output;
}

uint16_t Player::getMechanicsTotal()
{
    uint16_t result = 1 + downs + deaths;

    for(uint16_t index=0;index<tracker.size();index++)
    {
        result += tracker.at(index).hits;
    }
    return result;
}

void Player::resetStats()
{
    downs = 0;
    deaths = 0;
    is_downed = false;
    mechanics_failed = 0;
    mechanics_received = 0;
    last_stab_time = 0;
    last_hit_time=0;
    last_mechanic=0;

	for (auto mechanic = tracker.begin(); mechanic != tracker.end(); ++mechanic)
	{
		mechanic->hits = 0;
		mechanic->pulls = 0;
	}
}

void Player::addPull(Boss* new_boss)
{
    if(!in_squad) return;

    for(uint16_t index=0;index<tracker.size();index++)
    {
        tracker.at(index).addPull(new_boss);
    }
    pulls++;
}

void Player::combatEnter()
{
	in_combat = true;
}

void Player::combatExit()
{
	in_combat = false;
}

void Player::merge(Player * new_player)
{
	if (!new_player) return;
	if (new_player->id == id) return;

	for (uint16_t index = 0; index<new_player->tracker.size(); index++)
	{
		for (uint16_t index_hits = 0; index_hits<new_player->tracker.at(index).hits; index_hits++)
		{
			receiveMechanic(new_player->last_hit_time, new_player->tracker.at(index).name, new_player->tracker.at(index).id, new_player->tracker.at(index).fail, new_player->tracker.at(index).current_boss);
		}
	}
	downs += new_player->downs;
	deaths += new_player->deaths;
	pulls += new_player->pulls;
	new_player->resetStats();
}

void Player::MechanicTracker::addPull(Boss* new_boss)
{
    if(current_boss == new_boss)
    {
        pulls++;
    }
}

bool Player::MechanicTracker::isRelevant()
{
	return hits > 0;
}
