#include "mechanics.h"

bool have_added_line_break = true;
uint64_t last_mechanic_time = 0;
uint64_t line_break_frequency = 5000;
bool has_logged_mechanic = false;
DeimosOil deimos_oils[max_deimos_oils];

mechanic::mechanic()
{
    name = "";
    boss_id = 0;
    frequency_player = 2000;
    frequency_global = 0;
    last_hit_time = 0;
    is_interupt = false;
    is_multihit = true;
    target_is_dst = true;
    fail_if_hit = true;
	is_activation = 0;
	is_buffremove = 0;
	overstack_value = -1;
    valid_if_down = false;
	
	can_evade = true;
	can_block = true;
	can_invuln = true;

	verbosity = 2;

    special_requirement = requirementDefault;
	special_value = valueDefault;
}

float mechanic::isValidHit(Tracker* tracker, cbtevent* ev, ag* src, ag* dst)
{
    uint16_t index = 0;
    bool correct_id = false;
    Player* current_player = nullptr;

	if (!ev) return false;
	if (!src) return false;
	if (!dst) return false;

	if (can_block && ev->result == CBTR_BLOCK) return false;
	if (can_evade && ev->result == CBTR_EVADE) return false;
	if (can_invuln && ev->result == CBTR_ABSORB) return false;

	if (verbosity == 0) return false;

    for(index=0;index<ids.size();index++)
    {
        if(ev->skillid==this->ids[index])
        {
            correct_id = true;
            break;
        }
    }

	if (!correct_id) return false;

    if(frequency_global != 0
        && ev->time < last_hit_time+frequency_global-ms_per_tick)
    {
        return false;
    }

    if(boss_id
        && tracker->boss_found
        && !tracker->boss_data.hasId(boss_id))
    {
        return false;
    }

    if(ev->is_buffremove != is_buffremove)
    {
        return false;
    }

	if (ev->is_activation != is_activation)
	{
		return false;
	}

	if (is_buffremove
		&& overstack_value >= 0
		&& overstack_value != ev->overstack_value)
	{
		return false;
	}

    if(target_is_dst)
    {
        current_player= tracker->getPlayer(dst);
    }
    else
    {
        current_player= tracker->getPlayer(src);
    }

	if (!current_player) return false;

	if (is_multihit && ev->time < (current_player->getLastHitTime() + frequency_player)) return false;

	if (!valid_if_down && current_player->is_downed) return false;

	if (is_interupt && current_player->getLastStabTime() > ev->time) return false;

	if (!special_requirement(*this, ev, src, dst, current_player)) return false;

    current_player->setLastHitTime(ev->time);
    last_hit_time = ev->time;
    current_player->receiveMechanic(name,ids[0],fail_if_hit, &tracker->boss_data);

    return special_value(*this, ev, src, dst, current_player);
}

bool requirementDefault(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
    return true;
}

bool requirementConjure(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
    return dst->prof != 6;//not elementalist
}

bool requirementDhuumSnatch(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
    if((current_player->getLastHitTime() + current_mechanic.frequency_player) < ev->time)
    {
        current_player->setLastHitTime(ev->time);
        if(current_player->getLastMechanic() != MECHANIC_DHUUM_SNATCH)
        {
            return true;
        }
    }
    return false;
}

bool requirementDeimosOil(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
	DeimosOil current_oil;
	uint16_t oldest_index = max_deimos_oils-1;
	uint16_t current_index = 0;

	for (uint16_t index = 0; index < max_deimos_oils; index++)
	{
		if (deimos_oils[index].last_touch_time < current_oil.last_touch_time)
		{
			oldest_index = index;
		}
		if (deimos_oils[index].id == src->id)
		{
			current_oil = deimos_oils[index];
			current_index = index;
		}
	}

	if (!current_oil.id)
	{
		current_oil.id = src->id;
		current_oil.last_touch_time = ev->time;
		deimos_oils[oldest_index] = current_oil;
		return true;
	}
	else
	{
		deimos_oils[current_index].last_touch_time = ev->time;
		if ((ev->time - current_oil.last_touch_time) > current_mechanic.frequency_player)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool requirementOnSelf(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
	return src && dst && src->id == dst->id;
}

float valueDefault(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
	return 1;
}

float valueDhuumShackles(const mechanic & current_mechanic, cbtevent * ev, ag * src, ag * dst, Player * current_player)
{
	return (30000 - ev->value)/1000;
}

std::vector <mechanic> mechanics =
{
    mechanic().setName("was teleported").setIds({MECHANIC_VG_TELEPORT_RAINBOW,MECHANIC_VG_TELEPORT_GREEN}).setBossId(BOSS_VG_ID),

//  mechanic().setName("stood in the green circle").setIds({MECHANIC_VG_GREEN_A,MECHANIC_VG_GREEN_B,MECHANIC_VG_GREEN_C,MECHANIC_VG_GREEN_D}).setFailIfHit(false).setBossId(BOSS_GORS_ID).setCanInvuln(false),

	mechanic().setName("was slammed").setIds({MECHANIC_GORS_SLAM}).setIsInterupt(true).setBossId(BOSS_GORS_ID),

	mechanic().setName("was egged").setIds({MECHANIC_GORS_EGG}).setBossId(BOSS_GORS_ID),

	mechanic().setName("touched an orb").setIds({MECHANIC_GORS_ORB}).setBossId(BOSS_GORS_ID),

	mechanic().setName("got a sapper bomb").setIds({MECHANIC_SAB_SAPPER_BOMB}).setFailIfHit(false).setValidIfDown(true).setBossId(BOSS_SAB_ID),

	mechanic().setName("got a time bomb").setIds({MECHANIC_SAB_TIME_BOMB}).setFailIfHit(false).setValidIfDown(true).setBossId(BOSS_SAB_ID),

	mechanic().setName("stood in cannon fire").setIds({MECHANIC_SAB_CANNON}).setBossId(BOSS_SAB_ID),

	mechanic().setName("touched the flame wall").setIds({MECHANIC_SAB_FLAMEWALL}).setBossId(BOSS_SAB_ID),

	mechanic().setName("was hit with tantrum").setIds({MECHANIC_SLOTH_TANTRUM}).setBossId(BOSS_SLOTH_ID),

	mechanic().setName("got a bomb").setIds({MECHANIC_SLOTH_BOMB}).setFailIfHit(false).setFrequencyPlayer(6000).setBossId(BOSS_SLOTH_ID),

	mechanic().setName("stood in a bomb aoe").setIds({MECHANIC_SLOTH_BOMB_AOE}).setVerbosity(1).setBossId(BOSS_SLOTH_ID),

	mechanic().setName("was hit by flame breath").setIds({MECHANIC_SLOTH_FLAME_BREATH}).setBossId(BOSS_SLOTH_ID),

	mechanic().setName("was hit by shake").setIds({MECHANIC_SLOTH_SHAKE}).setBossId(BOSS_SLOTH_ID),

	mechanic().setName("is fixated").setIds({MECHANIC_SLOTH_FIXATE}).setFailIfHit(false).setBossId(BOSS_SLOTH_ID),

	mechanic().setName("was hadoukened").setIds({MECHANIC_MATT_HADOUKEN_HUMAN,MECHANIC_MATT_HADOUKEN_ABOM}).setBossId(BOSS_MATT_ID),

	mechanic().setName("reflected shards").setIds({MECHANIC_MATT_SHARD_HUMAN,MECHANIC_MATT_SHARD_ABOM}).setTargetIsDst(false).setBossId(BOSS_MATT_ID),

	mechanic().setName("got a bomb").setIds({MECHANIC_MATT_BOMB}).setFailIfHit(false).setFrequencyPlayer(12000).setBossId(BOSS_MATT_ID),

	mechanic().setName("got a corruption").setIds({MECHANIC_MATT_CORRUPTION}).setFailIfHit(false).setBossId(BOSS_MATT_ID),

	mechanic().setName("is sacrificed").setIds({MECHANIC_MATT_SACRIFICE}).setFailIfHit(false).setBossId(BOSS_MATT_ID),

	mechanic().setName("is east fixate").setIds({MECHANIC_KC_FIXATE_EAST}).setFailIfHit(false).setBossId(BOSS_KC_ID),
	mechanic().setName("is west fixate").setIds({MECHANIC_KC_FIXATE_WEST}).setFailIfHit(false).setBossId(BOSS_KC_ID),

//  mechanic().setName("stood in the red half").setIds({MECHANIC_XERA_HALF}).setBossId(BOSS_XERA_ID_A),

	mechanic().setName("has magic").setIds({MECHANIC_XERA_MAGIC}).setFailIfHit(false).setTargetIsDst(false).setFrequencyGlobal(12000).setValidIfDown(true).setBossId(BOSS_XERA_ID_A).setSpecialRequirement(requirementOnSelf),

	mechanic().setName("touched an orb").setIds({MECHANIC_XERA_ORB}).setBossId(BOSS_XERA_ID_A),

	mechanic().setName("stood in an orb aoe").setIds({MECHANIC_XERA_ORB_AOE}).setFrequencyPlayer(1000).setVerbosity(1).setBossId(BOSS_XERA_ID_A),

	mechanic().setName("was teleported").setIds({MECHANIC_CARIN_TELEPORT}).setBossId(BOSS_CAIRN_ID),

	mechanic().setName("was slapped").setIds({MECHANIC_CARIN_SWEEP}).setBossId(BOSS_CAIRN_ID),

//  mechanic().setName("reflected shards").setIds({MECHANIC_CARIN_SHARD}).setTargetIsDst(false).setBossId(BOSS_CAIRN_ID),

	mechanic().setName("missed a green circle").setIds({MECHANIC_CARIN_GREEN_A,MECHANIC_CARIN_GREEN_B,MECHANIC_CARIN_GREEN_C,MECHANIC_CARIN_GREEN_D,MECHANIC_CARIN_GREEN_E,MECHANIC_CARIN_GREEN_F}).setIsInterupt(true).setBossId(BOSS_CAIRN_ID),

	mechanic().setName("was hit by shockwave").setIds({MECHANIC_SAM_SHOCKWAVE}).setIsInterupt(true).setBossId(BOSS_SAM_ID),

	mechanic().setName("was horizontally slapped").setIds({MECHANIC_SAM_SLAP_HORIZONTAL}).setIsInterupt(true).setBossId(BOSS_SAM_ID),

	mechanic().setName("was vertically smacked").setIds({MECHANIC_SAM_SLAP_VERTICAL}).setIsInterupt(true).setBossId(BOSS_SAM_ID),

	mechanic().setName("is fixated").setIds({MECHANIC_SAM_FIXATE_SAM}).setFailIfHit(false).setBossId(BOSS_SAM_ID),

	mechanic().setName("has big green").setIds({MECHANIC_SAM_GREEN_BIG}).setFailIfHit(false).setBossId(BOSS_SAM_ID),
	mechanic().setName("has small green").setIds({MECHANIC_SAM_GREEN_SMALL}).setFailIfHit(false).setBossId(BOSS_SAM_ID),

	mechanic().setName("touched an oil").setIds({MECHANIC_DEIMOS_OIL}).setFrequencyPlayer(5000).setBossId(BOSS_DEIMOS_ID).setSpecialRequirement(requirementDeimosOil),

	mechanic().setName("was hit by smash").setIds({MECHANIC_DEIMOS_SMASH,MECHANIC_DEIMOS_SMASH_INITIAL,MECHANIC_DEIMOS_SMASH_END_A,MECHANIC_DEIMOS_SMASH_END_B}).setBossId(BOSS_DEIMOS_ID),

	mechanic().setName("closed a tear").setIds({MECHANIC_DEIMOS_TEAR}).setBossId(BOSS_DEIMOS_ID),

	mechanic().setName("stood in inner donut").setIds({MECHANIC_HORROR_DONUT_INNER}).setVerbosity(1).setBossId(BOSS_SH_ID),

	mechanic().setName("stood in outer donut").setIds({MECHANIC_HORROR_DONUT_OUTER}).setVerbosity(1).setBossId(BOSS_SH_ID),

	mechanic().setName("stood in torment aoe").setIds({MECHANIC_HORROR_GOLEM_AOE}).setBossId(BOSS_SH_ID),

	mechanic().setName("stood in pie slice").setIds({MECHANIC_HORROR_PIE_4_A,MECHANIC_HORROR_PIE_4_B}).setVerbosity(1).setBossId(BOSS_SH_ID),

	mechanic().setName("touched a scythe").setIds({MECHANIC_HORROR_SCYTHE}).setBossId(BOSS_SH_ID),

	mechanic().setName("took fixate").setIds({MECHANIC_HORROR_FIXATE}).setFailIfHit(false).setVerbosity(1).setBossId(BOSS_SH_ID),

	mechanic().setName("was debuffed").setIds({MECHANIC_HORROR_DEBUFF}).setFailIfHit(false).setVerbosity(1).setBossId(BOSS_SH_ID),

	mechanic().setName("was puked on").setIds({MECHANIC_EATER_PUKE}).setFrequencyPlayer(3000).setBossId(BOSS_SOUL_EATER_ID),

	mechanic().setName("stood in web").setIds({MECHANIC_EATER_WEB}).setFrequencyPlayer(3000).setBossId(BOSS_SOUL_EATER_ID),

	mechanic().setName("touched a messenger").setIds({MECHANIC_DHUUM_GOLEM}).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("is shackled").setIds({MECHANIC_DHUUM_SHACKLE}).setFailIfHit(false).setTargetIsDst(false).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("is shackled").setIds({MECHANIC_DHUUM_SHACKLE}).setFailIfHit(false).setBossId(BOSS_DHUUM_ID),

//	mechanic().setName("popped shackles").setIds({MECHANIC_DHUUM_SHACKLE}).setFailIfHit(false).setIsBuffremove(CBTB_MANUAL).setTargetIsDst(false).setSpecialValue(valueDhuumShackles).setBossId(BOSS_DHUUM_ID),
//	mechanic().setName("popped shackles").setIds({MECHANIC_DHUUM_SHACKLE}).setFailIfHit(false).setIsBuffremove(CBTB_MANUAL).setSpecialValue(valueDhuumShackles).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("has affliction").setIds({MECHANIC_DHUUM_AFFLICTION}).setFrequencyPlayer(13000 + ms_per_tick).setFailIfHit(false).setValidIfDown(true).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("stood in a crack").setIds({MECHANIC_DHUUM_CRACK}).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("stood in a mark").setIds({MECHANIC_DHUUM_MARK}).setVerbosity(0).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("touched the center").setIds({MECHANIC_DHUUM_SUCK_AOE}).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("stood in a teleport aoe").setIds({MECHANIC_DHUUM_TELEPORT_AOE}).setBossId(BOSS_DHUUM_ID),

//	mechanic().setName("died on green").setIds({MECHANIC_DHUUM_GREEN_TIMER}).setIsBuffremove(CBTB_MANUAL).setOverstackValue(0),

	mechanic().setName("was snatched").setIds({MECHANIC_DHUUM_SNATCH}).setSpecialRequirement(requirementDhuumSnatch).setBossId(BOSS_DHUUM_ID),

//	mechanic().setName("canceled button channel").setIds({MECHANIC_DHUUM_BUTTON_CHANNEL}).setIsActivation(ACTV_CANCEL_CANCEL).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("stood in cone").setIds({MECHANIC_DHUUM_CONE}).setBossId(BOSS_DHUUM_ID),

	mechanic().setName("vomited on someone").setIds({MECHANIC_NIGHTMARE_VOMIT}).setTargetIsDst(false),

	mechanic().setName("was hit by wirl").setIds({MECHANIC_MAMA_WIRL,MECHANIC_MAMA_WIRL_NORMAL}).setBossId(BOSS_MAMA_ID),

	mechanic().setName("was knocked").setIds({MECHANIC_MAMA_KNOCK}).setBossId(BOSS_MAMA_ID),

	mechanic().setName("was leaped on").setIds({MECHANIC_MAMA_LEAP}).setBossId(BOSS_MAMA_ID),

	mechanic().setName("stood in acid").setIds({MECHANIC_MAMA_ACID}).setBossId(BOSS_MAMA_ID),

	mechanic().setName("was smashed by a knight").setIds({MECHANIC_MAMA_KNIGHT_SMASH}).setBossId(BOSS_MAMA_ID),

	mechanic().setName("stood in acid").setIds({MECHANIC_SIAX_ACID}).setBossId(BOSS_SIAX_ID),

	mechanic().setName("was ran over").setIds({MECHANIC_ENSOLYSS_LUNGE}).setBossId(BOSS_ENSOLYSS_ID),

	mechanic().setName("was smashed").setIds({MECHANIC_ENSOLYSS_SMASH}).setBossId(BOSS_ENSOLYSS_ID),

	mechanic().setName("stood in a pie slice").setIds({MECHANIC_ARKK_PIE_A,MECHANIC_ARKK_PIE_B,MECHANIC_ARKK_PIE_C}).setBossId(BOSS_ARKK_ID),

//  mechanic().setName("was feared").setIds({MECHANIC_ARKK_FEAR}),

	mechanic().setName("was smashed").setIds({MECHANIC_ARKK_OVERHEAD_SMASH}).setBossId(BOSS_ARKK_ID),

	mechanic().setName("has a bomb").setIds({MECHANIC_ARKK_BOMB}).setFailIfHit(false).setBossId(BOSS_ARKK_ID),//TODO Add BOSS_ARTSARIIV_ID and make boss id a vector

//	mechanic().setName("didn't block the goop").setIds({MECHANIC_ARKK_GOOP}).setBossId(BOSS_ARKK_ID).setCanEvade(false),

#if 0//disable conjure detection due to potential toxicity
    mechanic().setName("picked up an ice bow").setIds({CONJURE_ICE_BOW_BUFF}).setFailIfHit(false).setSpecialRequirement(requirementConjure),

	mechanic().setName("picked up a lightning hammer").setIds({CONJURE_LIGHTNING_HAMMER_BUFF}).setFailIfHit(false).setSpecialRequirement(requirementConjure),

	mechanic().setName("picked up a flame axe").setIds({CONJURE_FLAME_AXE_BUFF}).setFailIfHit(false).setSpecialRequirement(requirementConjure),

	mechanic().setName("picked up an earth shield").setIds({CONJURE_EARTH_SHIELD_BUFF}).setFailIfHit(false).setSpecialRequirement(requirementConjure),

	mechanic().setName("picked up an FGS").setIds({CONJURE_FIRE_GS_BUFF}).setFailIfHit(false).setSpecialRequirement(requirementConjure)
#endif // 0
};
