#include "player.h"

std::vector<Player> players;
std::mutex players_mtx;

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
    downs--;
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
    for(uint16_t index=0;index<tracker.size();index++)
    {
        if(tracker.at(index).id == id)
        {
            tracker.at(index).hits++;
            return;
        }
    }
    std::lock_guard<std::mutex> lg(players_mtx);
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
        else if(new_player->name && std::string(new_player->name)==players.at(index).name)
        {
            players.at(index).id = new_player->id;
            return &players.at(index);
        }
    }

    std::lock_guard<std::mutex> lg(players_mtx);
    players.push_back(Player(new_player));
    return &players.back();
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
    players.clear();
}
