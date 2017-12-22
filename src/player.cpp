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
    last_machanic=0;       //skill id of last failed mechanic
}

Player::Player(ag* new_player)
{
    name = new_player->name;
    id = new_player->id;
    downs = 0;              //number of times the player has downed
    deaths = 0;
    is_downed = false;     //is currently is down state
    mechanics_failed = 0;   //number of mechanics failed
    mechanics_received = 0;
    last_stab_time = 0;  //time stability is going to expire
    last_hit_time=0;       //time player was last hit with a mechanic
    last_machanic=0;       //skill id of last failed mechanic
}

void Player::reset_stats()
{
    players_mtx.lock();
    downs = 0;
    deaths = 0;
    is_downed = false;
    mechanics_failed = 0;
    mechanics_received = 0;
    last_stab_time = 0;
    players_mtx.unlock();
}
void Player::reset_all()
{
    players_mtx.lock();
    name = "";
    id = 0;
    downs = 0;
    deaths = 0;
    is_downed = false;
    mechanics_failed = 0;
    mechanics_received = 0;
    last_stab_time = 0;
    players_mtx.unlock();
}

void Player::down()
{
    players_mtx.lock();
    downs++;
    is_downed = true;
    players_mtx.unlock();
}

void Player::dead()
{
    players_mtx.lock();
    deaths++;
    players_mtx.unlock();
}

void Player::rally()
{
    players_mtx.lock();
    is_downed = false;
    players_mtx.unlock();
}

void Player::set_stab_time(uint64_t new_stab_time)
{
    players_mtx.lock();
    if(last_stab_time < new_stab_time)
    {
        last_stab_time = new_stab_time;
    }
    players_mtx.unlock();
}

Player* get_player(ag* new_player)
{
    if(!is_player(new_player))
    {
        return nullptr;
    }
    players_mtx.lock();
    for(unsigned int index=0;index<players.size();index++)
    {
        if(players[index].id==0)
        {
            players[index] = Player(new_player);
            players_mtx.unlock();
            return &players[index];
        }
        else if(players[index].id == new_player->id || strcmp(new_player->name,players[index].name.c_str())==0)
        {
            players_mtx.unlock();
            return &players[index];
        }
    }

    players.push_back(Player(new_player));
    players_mtx.unlock();
    return &players.back();
}

bool is_player(ag* new_player)
{
    return new_player->prof < 10;
}

void reset_all_player_stats()
{
    players_mtx.lock();
    players.clear();
    players_mtx.unlock();
}
