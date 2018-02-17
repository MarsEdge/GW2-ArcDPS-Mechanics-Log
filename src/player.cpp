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
    is_downed = false;     //is currently is down state
    mechanics_failed = 0;   //number of mechanics failed
    mechanics_received = 0;
    last_stab_time = 0;  //time stability is going to expire
    last_hit_time=0;       //time player was last hit with a mechanic
    last_mechanic=0;       //skill id of last failed mechanic
    in_squad = true;
}

Player::Player(ag* &new_player)
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
    is_downed = false;     //is currently is down state
    mechanics_failed = 0;   //number of mechanics failed
    mechanics_received = 0;
    last_stab_time = 0;  //time stability is going to expire
    last_hit_time=0;       //time player was last hit with a mechanic
    last_mechanic=0;       //skill id of last failed mechanic
    in_squad = true;
}

Player::mechanic_tracker::mechanic_tracker(std::string &new_name,uint16_t &new_id,bool &new_fail)
{
    name = new_name;
    id = new_id;
    fail = new_fail;
    hits = 1;
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

void Player::fix_double_down()
{
    if(downs > 0) downs--;
}

void Player::mechanic_receive(std::string &name,uint16_t &id,bool &is_fail)
{
    if(!is_fail)
    {
        mechanics_received++;
    }
    else
    {
        mechanics_failed++;
    }
    set_last_mechanic(id);
    for(uint16_t index=0;index<tracker.size();index++)
    {
        if(tracker.at(index).id == id)
        {
            tracker.at(index).hits++;
            return;
        }
    }
    std::lock_guard<std::mutex> lg(tracker_mtx);
    tracker.push_back(mechanic_tracker(name,id,is_fail));
    return;
}

bool Player::is_relevant()
{
    return downs > 0
    || deaths > 0
    || mechanics_failed > 0
    || mechanics_received > 0;

}

uint64_t Player::get_last_stab_time()
{
    return last_stab_time;
}

void Player::set_stab_time(uint64_t new_stab_time)
{
    if(last_stab_time < new_stab_time)
    {
        last_stab_time = new_stab_time;
    }
}

uint64_t Player::get_last_hit_time()
{
    return last_hit_time;
}

void Player::set_last_hit_time(uint64_t new_hit_time)
{
    last_hit_time = new_hit_time;
}

uint16_t Player::get_last_mechanic()
{
    return last_mechanic;
}

void Player::set_last_mechanic(uint16_t new_mechanic)
{
    last_mechanic = new_mechanic;
}

std::string Player::mechanic_tracker::to_string()
{
    return name + "," +
    (!fail ? std::to_string(hits) + "," : "," + std::to_string(hits)) +
    "\n";
}

std::string Player::to_string()
{
    std::string output = "";

    output += name + "," + "Overall" + "," +
    std::to_string(mechanics_received) + "," +
    std::to_string(mechanics_failed) + "," +
    std::to_string(downs) + "," +
    std::to_string(deaths) + "," +
    std::to_string(pulls) +
    "\n";

    for(uint16_t index=0; index<tracker.size();index++)
    {
        output += name + "," +
        tracker.at(index).to_string();
    }
    return output;
}

uint16_t Player::get_mechanics_total()
{
    uint16_t result = 1 + downs + deaths;

    for(uint16_t index=0;index<tracker.size();index++)
    {
        result += tracker.at(index).hits;
    }
    return result;
}

void Player::reset_stats()
{
    downs = 0;
    deaths = 0;
    is_downed = false;
    mechanics_failed = 0;
    mechanics_received = 0;
    last_stab_time = 0;
    last_hit_time=0;
    last_mechanic=0;

    tracker.clear();
}

Player* get_player(ag* &new_player)
{
    if(!is_player(new_player))
    {
        return nullptr;
    }
    for(uint16_t index=0;index<players.size();index++)
    {
        if(players.at(index).id == new_player->id)
        {
            return &players.at(index);
        }
    }

    return nullptr;
}

void add_player(char* name, char* account, uintptr_t id)
{
    if(!name || !account)
    {
        return;
    }

    for(uint16_t index=0;index<players.size();index++)
    {
        if(players.at(index).id == id)
        {
            players.at(index).in_squad = true;
            return;
        }
        else if(name && account && std::string(account)==players.at(index).account)
        {
            players.at(index).id = id;
            players.at(index).name = name;
            players.at(index).in_squad = true;
            return;
        }
    }

    std::lock_guard<std::mutex> lg(players_mtx);
    players.push_back(Player(name, account, id));
}

void remove_player(char* name, char* account, uintptr_t id)
{
    if(!name || !account)
    {
        return;
    }

    for(uint16_t index=0;index<players.size();index++)
    {
        if(players.at(index).id == id)
        {
            players.at(index).in_squad = false;
            return;
        }
        else if(name && account && std::string(account)==players.at(index).account)
        {
            players.at(index).id = id;
            players.at(index).name = name;
            players.at(index).in_squad = false;
            return;
        }
    }
}

void add_pull()
{
    for(uint16_t index=0;index<players.size();index++)
    {
        if(players.at(index).in_squad)
        {
            players.at(index).pulls++;
        }
    }
}

bool is_player(ag* &new_player)
{
    return new_player
    && new_player->prof > 0
    && new_player->prof < 10
    && new_player->name != nullptr
    && strlen(new_player->name) > 2;
}

void reset_all_player_stats()
{
    std::lock_guard<std::mutex> lg(players_mtx);
    for(uint16_t index=0;index<players.size();index++)
    {
        players.at(index).reset_stats();
    }
}

uint16_t get_mechanics_total(std::vector<Player> &players)
{
    uint16_t result = 0;

    for(uint16_t index=0;index<players.size();index++)
    {
        if(players.at(index).is_relevant())
        {
            result += players.at(index).get_mechanics_total();
        }
    }
    return result;
}
