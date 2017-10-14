#include "player.h"

Player players[MAX_PLAYER_COUNT] = {Player()};

Player::Player()
{
    id = 0;            //instance id
    downs = 0;              //number of times the player has downed
    is_downed = false;     //is currently is down state
    mechanics_failed = 0;   //number of mechanics failed
    last_stab_time = 0;  //time stability is going to expire
    last_hit_time=0;       //time player was last hit with a mechanic
    last_machanic=0;       //skill id of last failed mechanic
}

Player::Player(uint16_t new_id)
{
    id = new_id;
    downs = 0;              //number of times the player has downed
    is_downed = false;     //is currently is down state
    mechanics_failed = 0;   //number of mechanics failed
    last_stab_time = 0;  //time stability is going to expire
    last_hit_time=0;       //time player was last hit with a mechanic
    last_machanic=0;       //skill id of last failed mechanic
}
void Player::reset_stats()
{
    downs = 0;
    is_downed = false;
    mechanics_failed = 0;
    last_stab_time = 0;
}
void Player::reset_all()
{
    id = 0;
    downs = 0;
    is_downed = false;
    mechanics_failed = 0;
    last_stab_time = 0;
}

Player* get_player(uint16_t new_id)
{
    for(unsigned int index=0;index<MAX_PLAYER_COUNT;index++)
    {
        if(players[index].id==0)
        {
            players[index] = Player(new_id);
            return &players[index];
        }
        else if(players[index].id == new_id)
        {
            return &players[index];
        }
    }
    return nullptr;
}

bool is_player(ag* new_player)
{
    return new_player->prof < 10;
}

void reset_all_player_stats()
{
    for(unsigned int index=0;index<MAX_PLAYER_COUNT;index++)
    {
        players[index].reset_all();
    }
}
