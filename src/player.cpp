#include "player.h"

std::vector<Player> players;
std::mutex players_mtx;
std::mutex tracker_mtx;

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
}

Player::Player(char* new_name, char* new_account, uintptr_t new_id, bool new_is_self)
{
    name = new_name ? new_name : "";
    account = new_account ? new_account : "";
    id = new_id;
	is_self = new_is_self;
}

bool Player::operator==(uintptr_t other_id)
{
	return id == other_id;
}

bool Player::operator==(std::string other_str)
{
	return name == other_str || account == other_str;
}
