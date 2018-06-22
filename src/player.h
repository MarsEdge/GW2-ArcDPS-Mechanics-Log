#pragma once

#include <stdint.h>
#include <string>
#include <cstring>
#include <vector>
#include <mutex>
#include "arcdps_datastructures.h"
#include "npc_ids.h"

struct Player
{
    std::string name;
    std::string account;
    uintptr_t id;            //instance id
    uint16_t downs;              //number of times the player has downed
    uint16_t deaths;              //number of times the player has completely died
    uint16_t pulls;         //number of boss pulls
    bool is_downed;     //is currently is down state
    uint16_t mechanics_failed;   //number of mechanics failed
    uint16_t mechanics_received;//number of boss mechanics received
    uint64_t last_stab_time;  //time stability is going to expire
    uint64_t last_hit_time;       //time player was last hit with a mechanic
    uint16_t last_mechanic;       //skill id of last failed mechanic
    bool in_squad;          //currently in squad

    struct MechanicTracker
    {
        std::string name; //name of mechanic
        uint16_t id; //skill id;
        bool fail;
        uint16_t hits;
        Boss* current_boss; //boss
        uint16_t pulls;

        MechanicTracker(std::string &new_name,uint16_t &new_id,bool &new_fail, Boss* new_boss);

        std::string toString();
        void addPull(Boss* new_boss);
    };

    std::vector<MechanicTracker> tracker;

    Player();
    Player(ag* new_player);
    Player(char* new_name, char* new_account, uintptr_t new_id);

    void down();
    void dead();
    void rally();
    void fixDoubleDown(); //manual case to fix vapor form counting as 2 downs

    void receiveMechanic(std::string &name,uint16_t &id,bool &is_fail, Boss* boss);
    bool isRelevant();     //if player is relevant for displaying

    uint64_t getLastStabTime();
    void setStabTime(uint64_t new_stab_time);

    uint64_t getLastHitTime();
    void setLastHitTime(uint64_t new_hit_time);

    uint16_t getLastMechanic();
    void setLastMechanic(uint16_t new_mechanic);

    std::string toString();
    uint16_t getMechanicsTotal();//returns the number of total mechanics the player had
    void resetStats();
    void addPull(Boss* new_boss);

	void merge(Player* new_player);
};

extern std::mutex players_mtx;
extern std::mutex tracker_mtx;

extern std::vector<Player> players;

Player* getPlayer(ag* new_player);
void addPlayer(char* name, char* account, uintptr_t id);
void removePlayer(char* name, char* account, uintptr_t id);
void addPull(Boss* boss);
bool isPlayer(ag* new_player);
void resetAllPlayerStats();
uint16_t getMechanicsTotal();
