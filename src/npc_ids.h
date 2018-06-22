#pragma once

#include <stdint.h>
#include <vector>
#include <string>

const uint32_t BOSS_VG_ID               = 0x3C4E;
const uint32_t BOSS_VG_HEALTH           = 22021440;
const uint32_t BOSS_VG_TIMER            = 8*60*1000;

const uint32_t BOSS_GORS_ID             = 0x3C45;
const uint32_t BOSS_GORS_HEALTH         = 21628200;
const uint32_t BOSS_GORS_TIMER          = 7*60*1000;

const uint32_t BOSS_SAB_ID              = 0x3C0F;
const uint32_t BOSS_SAB_HEALTH          = 34015256;
const uint32_t BOSS_SAB_TIMER           = 9*60*1000;


const uint32_t BOSS_SLOTH_ID            = 0x3EFB;
const uint32_t BOSS_SLOTH_HEALTH        = 18973828;
const uint32_t BOSS_SLOTH_TIMER         = 7*60*1000;

const uint32_t BOSS_TRIO_BURG_ID        = 0x3ED8;
const uint32_t BOSS_TRIO_ZANE_ID        = 0x3F09;
const uint32_t BOSS_TRIO_NARELLA_ID     = 0x3EFD;
const uint32_t BOSS_TRIO_TIMER          = 7*60*1000;

const uint32_t BOSS_MATT_ID             = 0x3EF3;
const uint32_t BOSS_MATT_HEALTH         = 25953840;
const uint32_t BOSS_MATT_TIMER          = 10*60*1000;

const uint32_t BOSS_KC_ID               = 0x3F6B;
const uint32_t BOSS_KC_HEALTH           = 55053600;
const uint32_t BOSS_KC_TIMER            = 10*60*1000;

const uint32_t BOSS_XERA_ID_A           = 0x3F76;
const uint32_t BOSS_XERA_ID_B           = 0x3F9E;
const uint32_t BOSS_XERA_HEALTH         = 22611300;
const uint32_t BOSS_XERA_TIMER          = 11*60*1000;


const uint32_t BOSS_CAIRN_ID            = 0x432A;
const uint32_t BOSS_CAIRN_HEALTH        = 19999998;
const uint32_t BOSS_CAIRN_TIMER         = 8*60*1000;

const uint32_t BOSS_MO_ID               = 0x4314;
const uint32_t BOSS_MO_HEALTH           = 22021440;
const uint32_t BOSS_MO_TIMER            = 6*60*1000;

const uint32_t BOSS_SAM_ID              = 0x4324;
const uint32_t BOSS_SAM_HEALTH          = 29493000;
const uint32_t BOSS_SAM_TIMER           = 11*60*1000;

const uint32_t BOSS_DEIMOS_ID           = 0x4302;
const uint32_t BOSS_DEIMOS_HEALTH       = 50049000;
const uint32_t BOSS_DEIMOS_TIMER        = 12*60*1000;


const uint32_t BOSS_SH_ID               = 0x4D37;
const uint32_t BOSS_SH_HEALTH           = 35391600;
const uint32_t BOSS_SH_TIMER            = 8*60*1000;

const uint32_t BOSS_SOUL_EATER_ID		= 0x4C50;
const uint32_t BOSS_SOUL_EATER_HEALTH	= 1720425;

const uint32_t BOSS_ICE_KING_ID			= 0x4CEB;
const uint32_t BOSS_ICE_KING_TIMER		= (3 * 60 + 30) * 1000;
const uint32_t BOSS_ICE_KING_HEALTH		= 9831000;

const uint32_t BOSS_EYE_NORTH_ID		= 0x4CC3;//Eye of Judgement
const uint32_t BOSS_EYE_SOUTH_ID		= 0x4D84;//Eye of Fate
const uint32_t BOSS_EYE_HEALTH			= 2457750;

const uint32_t BOSS_DHUUM_ID            = 0x4BFA;
const uint32_t BOSS_DHUUM_HEALTH        = 32000000;
const uint32_t BOSS_DHUUM_TIMER         = 10*60*1000;


const uint32_t BOSS_MAMA_ID             = 0x427D;
const uint32_t BOSS_MAMA_ID_NORMAL      = 0x4268;
const uint32_t BOSS_MAMA_ID_TRASH       = 0x424E;//knight at the start of the trash on CM
const uint64_t BOSS_MAMA_HEALTH         = 5200519;

const uint32_t BOSS_SIAX_ID             = 0x4284;
const uint64_t BOSS_SIAX_HEALTH         = 6138797;

const uint32_t BOSS_ENSOLYSS_ID         = 0x4234;
const uint64_t BOSS_ENSOLYSS_HEALTH     = 14059890;


const uint32_t BOSS_SKORVALD_ID        = 0x44E0;
const uint64_t BOSS_SKORVALD_HEALTH    = 5551340;

const uint32_t BOSS_ARTSARIIV_ID       = 0x461D;
const uint64_t BOSS_ARTSARIIV_HEALTH   = 5962266;

const uint32_t BOSS_ARKK_ID            = 0x455F;
const uint64_t BOSS_ARKK_HEALTH        = 9942250;

struct Boss
{
    std::vector<uint32_t> ids;//npc id (from ag->prof)
    std::string name;
    int64_t timer;//enrage timer
    uint64_t health;
	uint16_t pulls;

    Boss();

    bool hasId(uint32_t new_id);

    Boss setIds(std::initializer_list<uint32_t> const new_ids) {this->ids = std::vector<uint32_t>(new_ids); return *this;}
    Boss setName(std::string const new_name) {this->name = new_name; return *this;}
    Boss setTimer(int64_t const new_timer) {this->timer = new_timer; return *this;}
    Boss setHealth(uint64_t const new_health) {this->health = new_health; return *this;}
};

extern std::vector<Boss> bosses;
