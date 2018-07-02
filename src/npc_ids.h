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

const Boss boss_vg = Boss().setIds({ 0x3C4E }).setTimer(8 * 60 * 1000).setHealth(22021440).setName("Vale Guardian");

const Boss boss_gors = Boss().setIds({ 0x3C45 }).setTimer(7 * 60 * 1000).setHealth(21628200).setName("Gorseval the Multifarious");

const Boss boss_sab = Boss().setIds({ 0x3C0F }).setTimer(9 * 60 * 1000).setHealth(34015256).setName("Sabetha the Saboteur");

const Boss boss_sloth = Boss().setIds({ 0x3EFB }).setTimer(7 * 60 * 1000).setHealth(18973828).setName("Slothasor");

const Boss boss_trio = Boss().setIds({ 0x3ED8,0x3F09,0x3EFD }).setTimer(7 * 60 * 1000).setName("Bandit Trio");//TODO: verify enrage timer

const Boss boss_matti = Boss().setIds({ 0x3EF3 }).setTimer(10 * 60 * 1000).setHealth(25953840).setName("Matthias Gabrel");//TODO: verify enrage timer


//TODO: Add escort boss

const Boss boss_kc = Boss().setIds({ 0x3F6B }).setTimer(10 * 60 * 1000).setHealth(55053600).setName("Keep Construct");//TODO: verify enrage timer

const Boss boss_xera = Boss().setIds({ 0x3F76,0x3F9E }).setTimer(11 * 60 * 1000).setHealth(22611300).setName("Xera");//TODO: verify enrage timer


const Boss boss_cairn = Boss().setIds({ 0x432A }).setTimer(8 * 60 * 1000).setHealth(19999998).setName("Cairn the Indomitable");//TODO: verify enrage timer

const Boss boss_mo = Boss().setIds({ 0x4314 }).setTimer(6 * 60 * 1000).setHealth(22021440).setName("Mursaat Overseer");//TODO: verify enrage timer

const Boss boss_sam = Boss().setIds({ 0x4324 }).setTimer(11 * 60 * 1000).setHealth(29493000).setName("Samarog");//TODO: verify enrage timer

const Boss boss_deimos = Boss().setIds({ 0x4302 }).setTimer(12 * 60 * 1000).setHealth(50049000).setName("Deimos");//TODO: verify enrage timer


const Boss boss_sh = Boss().setIds({ 0x4D37 }).setTimer(8 * 60 * 1000).setHealth(35391600).setName("Soulless Horror");

//TODO: Add soul river boss

const Boss boss_soul_eater = Boss().setIds({ 0x4C50 }).setHealth(1720425).setName("Statues - Soul Eater");

const Boss boss_ice_king = Boss().setIds({ 0x4CEB }).setTimer((3 * 60 + 30) * 1000).setHealth(9831000).setName("Statues - Ice King");

const Boss boss_cave = Boss().setIds({ 0x4CC3,0x4D84 }).setHealth(2457750).setName("Statues - Cave");//North: Eye of Judgement, South: Eye of Fate

const Boss boss_dhuum = Boss().setIds({ 0x4BFA }).setTimer(10 * 60 * 1000).setHealth(32000000).setName("Dhuum");


const Boss boss_mama = Boss().setIds({ 0x427D,0x4268,0x424E }).setHealth(5200519).setName("MAMA");//ids: cm,normal,knight at the start of the trash on CM

const Boss boss_siax = Boss().setIds({ 0x4284 }).setHealth(6138797).setName("Siax");//TODO get normal mode id

const Boss boss_ensolyss = Boss().setIds({ 0x4234 }).setHealth(14059890).setName("Ensolyss of the Endless Torment");//TODO get normal mode id


const Boss boss_skorvald = Boss().setIds({ 0x44E0 }).setHealth(5551340).setName("Skorvald the Shattered");//TODO get normal mode id

const Boss boss_artsariiv = Boss().setIds({ 0x461D }).setHealth(5962266).setName("Artsariiv");//TODO get normal mode id

const Boss boss_arkk = Boss().setIds({ 0x455F }).setHealth(9942250).setName("Arkk");//TODO get normal mode id

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






