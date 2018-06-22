#pragma once

#include <stdint.h>
#include <vector>
#include <string>

class Boss
{
public:
	std::vector<uint32_t> ids;//npc id (from ag->prof)
	std::string name;
	int64_t timer;//enrage timer
	uint64_t health;
	uint16_t pulls;

	Boss();

	bool hasId(uint32_t new_id);

	Boss setIds(std::initializer_list<uint32_t> const new_ids) { this->ids = std::vector<uint32_t>(new_ids); return *this; }
	Boss setName(std::string const new_name) { this->name = new_name; return *this; }
	Boss setTimer(int64_t const new_timer) { this->timer = new_timer; return *this; }
	Boss setHealth(uint64_t const new_health) { this->health = new_health; return *this; }
};

const Boss boss_vg = Boss().setIds({ 0x3C4E }).setTimer(8 * 60 * 1000).setHealth(22021440);

const Boss boss_gors = Boss().setIds({ 0x3C45 }).setTimer(7 * 60 * 1000).setHealth(21628200);

const Boss boss_sab = Boss().setIds({ 0x3C0F }).setTimer(9 * 60 * 1000).setHealth(34015256);

const Boss boss_sloth = Boss().setIds({ 0x3EFB }).setTimer(7 * 60 * 1000).setHealth(18973828);

const Boss boss_trio = Boss().setIds({ 0x3ED8,0x3F09,0x3EFD }).setTimer(7 * 60 * 1000);//TODO: verify enrage timer

const Boss boss_matti = Boss().setIds({ 0x3EF3 }).setTimer(10 * 60 * 1000).setHealth(25953840);//TODO: verify enrage timer


//TODO: Add escort boss

const Boss boss_kc = Boss().setIds({ 0x3F6B }).setTimer(10 * 60 * 1000).setHealth(55053600);//TODO: verify enrage timer

const Boss boss_xera = Boss().setIds({ 0x3F76,0x3F9E }).setTimer(11 * 60 * 1000).setHealth(22611300);//TODO: verify enrage timer


const Boss boss_cairn = Boss().setIds({ 0x432A }).setTimer(8 * 60 * 1000).setHealth(19999998);//TODO: verify enrage timer

const Boss boss_mo = Boss().setIds({ 0x4314 }).setTimer(6 * 60 * 1000).setHealth(22021440);//TODO: verify enrage timer

const Boss boss_sam = Boss().setIds({ 0x4324 }).setTimer(11 * 60 * 1000).setHealth(29493000);//TODO: verify enrage timer

const Boss boss_deimos = Boss().setIds({ 0x4302 }).setTimer(12 * 60 * 1000).setHealth(50049000);//TODO: verify enrage timer


const Boss boss_sh = Boss().setIds({ 0x4D37 }).setTimer(8 * 60 * 1000).setHealth(35391600);

//TODO: Add soul river boss

const Boss boss_soul_eater = Boss().setIds({ 0x4C50 }).setHealth(1720425);

const Boss boss_ice_king = Boss().setIds({ 0x4CEB }).setTimer((3 * 60 + 30) * 1000).setHealth(9831000);

const Boss boss_cave = Boss().setIds({ 0x4CC3,0x4D84 }).setHealth(2457750);//North: Eye of Judgement, South: Eye of Fate

const Boss boss_dhuum = Boss().setIds({ 0x4BFA }).setTimer(10 * 60 * 1000).setHealth(32000000);


const Boss boss_mama = Boss().setIds({ 0x427D,0x4268,0x424E }).setHealth(5200519);//ids: cm,normal,knight at the start of the trash on CM

const Boss boss_siax = Boss().setIds({ 0x4284 }).setHealth(6138797);//TODO get normal mode id

const Boss boss_ensolyss = Boss().setIds({ 0x4234 }).setHealth(14059890);//TODO get normal mode id


const Boss boss_skorvald = Boss().setIds({ 0x44E0 }).setHealth(5551340);//TODO get normal mode id

const Boss boss_artsariiv = Boss().setIds({ 0x461D }).setHealth(5962266);//TODO get normal mode id

const Boss boss_arkk = Boss().setIds({ 0x455F }).setHealth(9942250);//TODO get normal mode id

const std::vector<const Boss*> bosses =
{
	&boss_vg,
	&boss_gors,
	&boss_sab,
	&boss_sloth,
	&boss_trio,
	&boss_matti,
	&boss_kc,
	&boss_xera,
	&boss_cairn,
	&boss_mo,
	&boss_sam,
	&boss_deimos,
	&boss_sh,
	&boss_soul_eater,
	&boss_ice_king,
	&boss_cave,
	&boss_dhuum,
	&boss_mama,
	&boss_siax,
	&boss_ensolyss,
	&boss_skorvald,
	&boss_artsariiv,
	&boss_arkk,
};






