#include "npc_ids.h"

Boss::Boss()
{
    ids = std::vector<uint32_t>();
    name = "";
    timer = 0;
    health = 0;
	pulls = 0;
}

bool Boss::hasId(uint32_t new_id)
{
    for(uint16_t index=0;index<ids.size();index++)
    {
        if(new_id == ids.at(index))
        {
            return true;
        }
    }
    return false;
}

bool Boss::operator==(Boss * other_boss)
{
	return other_boss && ids.size()>0 && other_boss->ids.size()>0 
		&& ids[0] == other_boss->ids[0];
}

Boss boss_generic = Boss().setName("Generic");
Boss boss_vg = Boss().setIds({ 0x3C4E }).setTimer(8 * 60 * 1000).setHealth(22021440).setName("Vale Guardian");
Boss boss_gors = Boss().setIds({ 0x3C45 }).setTimer(7 * 60 * 1000).setHealth(21628200).setName("Gorseval the Multifarious");
Boss boss_sab = Boss().setIds({ 0x3C0F }).setTimer(9 * 60 * 1000).setHealth(34015256).setName("Sabetha the Saboteur");
Boss boss_sloth = Boss().setIds({ 0x3EFB }).setTimer(7 * 60 * 1000).setHealth(18973828).setName("Slothasor");
Boss boss_trio = Boss().setIds({ 0x3ED8,0x3F09,0x3EFD }).setTimer(7 * 60 * 1000).setName("Bandit Trio");//TODO: verify enrage timer
Boss boss_matti = Boss().setIds({ 0x3EF3 }).setTimer(10 * 60 * 1000).setHealth(25953840).setName("Matthias Gabrel");//TODO: verify enrage timer
Boss boss_kc = Boss().setIds({ 0x3F6B }).setTimer(10 * 60 * 1000).setHealth(55053600).setName("Keep Construct");//TODO: verify enrage timer
Boss boss_xera = Boss().setIds({ 0x3F76,0x3F9E }).setTimer(11 * 60 * 1000).setHealth(22611300).setName("Xera");//TODO: verify enrage timer
Boss boss_cairn = Boss().setIds({ 0x432A }).setTimer(8 * 60 * 1000).setHealth(19999998).setName("Cairn the Indomitable");//TODO: verify enrage timer
Boss boss_mo = Boss().setIds({ 0x4314 }).setTimer(6 * 60 * 1000).setHealth(22021440).setName("Mursaat Overseer");//TODO: verify enrage timer
Boss boss_sam = Boss().setIds({ 0x4324 }).setTimer(11 * 60 * 1000).setHealth(29493000).setName("Samarog");//TODO: verify enrage timer
Boss boss_deimos = Boss().setIds({ 0x4302 }).setTimer(12 * 60 * 1000).setHealth(50049000).setName("Deimos");//TODO: verify enrage timer
Boss boss_sh = Boss().setIds({ 0x4D37 }).setTimer(8 * 60 * 1000).setHealth(35391600).setName("Soulless Horror");
Boss boss_soul_eater = Boss().setIds({ 0x4C50 }).setHealth(1720425).setName("Statues - Soul Eater");
Boss boss_ice_king = Boss().setIds({ 0x4CEB }).setTimer((3 * 60 + 30) * 1000).setHealth(9831000).setName("Statues - Ice King");
Boss boss_cave = Boss().setIds({ 0x4CC3,0x4D84 }).setHealth(2457750).setName("Statues - Cave");//North: Eye of Judgement, South: Eye of Fate
Boss boss_dhuum = Boss().setIds({ 0x4BFA }).setTimer(10 * 60 * 1000).setHealth(32000000).setName("Dhuum");
Boss boss_ca = Boss().setIds({ 43974,37464,10142 }).setTimer(8 * 60 * 1000).setName("Conjured Amalgamate");//TODO: Get boss HP
Boss boss_largos = Boss().setIds({ 21105, 21089 }).setTimer(2 * 4 * 60 * 1000).setHealth(17548336).setName("Twin Largos");//using Nikare (left side hp)
Boss boss_qadim = Boss().setIds({ 20934 }).setTimer(13 * 60 * 1000).setHealth(19268760).setName("Qadim");
Boss boss_adina = Boss().setIds({ 22006 }).setTimer(8 * 60 * 1000).setHealth(22611300).setName("Adina");
Boss boss_sabir = Boss().setIds({ 21964 }).setHealth(29493000).setName("Sabir");
Boss boss_qadim2 = Boss().setIds({ 22000 }).setTimer(12 * 60 * 1000).setHealth(47188800).setName("Qadim the Peerless");
Boss boss_fotm_generic = Boss().setName("FotM Generic");
Boss boss_mama = Boss().setIds({ 0x427D,0x4268,0x424E }).setHealth(5200519).setName("MAMA");//ids: cm,normal,knight at the start of the trash on CM
Boss boss_siax = Boss().setIds({ 0x4284 }).setHealth(6138797).setName("Siax");//TODO get normal mode id
Boss boss_ensolyss = Boss().setIds({ 0x4234 }).setHealth(14059890).setName("Ensolyss of the Endless Torment");//TODO get normal mode id
Boss boss_skorvald = Boss().setIds({ 0x44E0 }).setHealth(5551340).setName("Skorvald the Shattered");//TODO get normal mode id
Boss boss_artsariiv = Boss().setIds({ 0x461D }).setHealth(5962266).setName("Artsariiv");//TODO get normal mode id
Boss boss_arkk = Boss().setIds({ 0x455F }).setHealth(9942250).setName("Arkk");//TODO get normal mode id
Boss boss_strike_generic = Boss().setName("Strike Generic");
Boss boss_boneskinner = Boss().setIds({ 22521 }).setName("Boneskinner");//TODO get timer & health
Boss boss_fraenir = Boss().setIds({ 22492 }).setName("Fraenir");//TODO get timer & health
Boss boss_kodan = Boss().setIds({ 22343, 22481, 22315 }).setName("Kodan Brothers");//TODO get timer & health
Boss boss_whisper = Boss().setIds({ 22711 }).setName("Whisper of Jormag");//TODO get timer & health
Boss boss_icebrood_construct = Boss().setIds({ 22154, 22436 }).setName("Icebrood Construct");//TODO get timer & health

std::list<Boss*> bosses =
{
	&boss_generic,
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
	&boss_ca,
	&boss_largos,
	&boss_qadim,
	&boss_fotm_generic,
	&boss_mama,
	&boss_siax,
	&boss_ensolyss,
	&boss_skorvald,
	&boss_artsariiv,
	&boss_arkk,
	&boss_boneskinner,
	&boss_fraenir,
	&boss_kodan,
	&boss_whisper,
	&boss_icebrood_construct,
};
