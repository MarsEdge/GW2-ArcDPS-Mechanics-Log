#include "mechanics.h"

bool has_logged_mechanic = false;
DeimosOil deimos_oils[max_deimos_oils];
EndersEcho enders_echo;

Mechanic::Mechanic() noexcept
{
	special_requirement = requirementDefault;
	special_value = valueDefault;
}

int64_t Mechanic::isValidHit(cbtevent* ev, Player* src, Player* dst)
{
    uint16_t index = 0;
    bool correct_id = false;
    Player* current_player = nullptr;

	if (!ev) return false;
	if (!src && !dst) return false;

	if (can_block && ev->result == CBTR_BLOCK) return false;
	if (can_evade && ev->result == CBTR_EVADE) return false;
	if (can_invuln && ev->result == CBTR_ABSORB) return false;

	if (!verbosity) return false;

    for(index=0;index<ids_size;index++)
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

    if(ev->is_buffremove != is_buffremove)
    {
        return false;
    }

	if (is_activation)
	{//Normal and quickness activations are interchangable
		if (is_activation == ACTV_NORMAL
			|| is_activation == ACTV_QUICKNESS)
		{
			if (ev->is_activation != ACTV_NORMAL
				&& ev->is_activation != ACTV_QUICKNESS)
			{
				return false;
			}
		}
	}

	if (is_buffremove//TODO: this check is wrong. overstack does not require buffremove
		&& overstack_value >= 0
		&& overstack_value != ev->overstack_value)
	{
		return false;
	}

	if (value != -1
		&& ev->value != value)
	{
		return false;
	}

    if(target_is_dst)
    {
        current_player = dst;
    }
    else
    {
        current_player = src;
    }

	if (!current_player) return false;

	if (!valid_if_down && current_player->is_downed) return false;

	if (is_interupt && current_player->last_stab_time > ev->time) return false;

	if (!special_requirement(*this, ev, src, dst, current_player)) return false;

    last_hit_time = ev->time;

    return special_value(*this, ev, src, dst, current_player);
}

std::string Mechanic::getIniName()
{
	return std::to_string(ids[0])
		+ " - " + (boss ? boss->name : "")
		+ " - " + name;
}

bool requirementDefault(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player)
{
    return true;
}

bool requirementDhuumSnatch(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player)
{
	for (auto current_pair = enders_echo.players_snatched.begin(); current_pair != enders_echo.players_snatched.end(); ++current_pair)
	{
		//if player has been snatched before and is in tracking
		if (ev->dst_instid == current_pair->first)
		{
			if ((current_pair->second + current_mechanic.frequency_player) > ev->time)
			{
				current_pair->second = ev->time;
				return false;
			}
			else
			{
				current_pair->second = ev->time;
				return true;
			}
		}
	}
	
	//if player not seen before
	enders_echo.players_snatched.push_back(std::pair<uint16_t, uint64_t>(ev->dst_instid, ev->time));
	return true;
}

bool requirementBuffApply(const Mechanic & current_mechanic, cbtevent * ev, Player * src, Player * dst, Player * current_player)
{
	return ev
		&& ev->buff
		&& ev->buff_dmg==0;
}

bool requirementDeimosOil(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player)
{
	DeimosOil* current_oil = nullptr;
	DeimosOil* oldest_oil = &deimos_oils[0];

	//find if the oil is already tracked
	for (auto index = 0; index < max_deimos_oils; index++)
	{
		if (deimos_oils[index].last_touch_time < oldest_oil->last_touch_time)//find oldest oil
		{
			oldest_oil = &deimos_oils[index];
		}
		if (deimos_oils[index].id == ev->src_instid)//if oil is already known
		{
			current_oil = &deimos_oils[index];
		}
	}

	//if oil is new
	if (!current_oil)
	{
		current_oil = oldest_oil;
		current_oil->id = ev->src_instid;
		current_oil->first_touch_time = ev->time;
		current_oil->last_touch_time = ev->time;
		return true;
	}
	else
	{//oil is already known
		current_oil->last_touch_time = ev->time;
		if ((ev->time - current_oil->last_touch_time) > current_mechanic.frequency_player)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool requirementOnSelf(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player)
{
	return ev->src_instid == ev->dst_instid;
}

int64_t valueDefault(const Mechanic &current_mechanic, cbtevent* ev, Player* src, Player* dst, Player* current_player)
{
	return 1;
}

int64_t valueDhuumShackles(const Mechanic & current_mechanic, cbtevent * ev, Player * src, Player * dst, Player * current_player)
{
	return (30000 - ev->value)/1000;
}

std::vector <Mechanic> mechanics =
{
    Mechanic().setName("was teleported").setIds({MECHANIC_VG_TELEPORT_RAINBOW,MECHANIC_VG_TELEPORT_GREEN}).setBoss(&boss_vg),

	Mechanic().setName("stood in the green circle").setIds({MECHANIC_VG_GREEN_A,MECHANIC_VG_GREEN_B,MECHANIC_VG_GREEN_C,MECHANIC_VG_GREEN_D}).setFailIfHit(false).setBoss(&boss_vg).setCanInvuln(false).setVerbosity(verbosity_chart),

	Mechanic().setName("was slammed").setIds({MECHANIC_GORS_SLAM}).setIsInterupt(true).setBoss(&boss_gors),

	Mechanic().setName("was egged").setIds({MECHANIC_GORS_EGG}).setBoss(&boss_gors),

	Mechanic().setName("touched an orb").setIds({MECHANIC_GORS_ORB}).setBoss(&boss_gors).setSpecialRequirement(requirementBuffApply),

	Mechanic().setName("got a sapper bomb").setIds({MECHANIC_SAB_SAPPER_BOMB}).setFailIfHit(false).setValidIfDown(true).setBoss(&boss_sab),

	Mechanic().setName("got a time bomb").setIds({MECHANIC_SAB_TIME_BOMB}).setFailIfHit(false).setValidIfDown(true).setBoss(&boss_sab),

	Mechanic().setName("stood in cannon fire").setIds({MECHANIC_SAB_CANNON}).setBoss(&boss_sab),

//	Mechanic().setName("touched the flame wall").setIds({MECHANIC_SAB_FLAMEWALL}).setBoss(&boss_sab),

	Mechanic().setName("was hit with tantrum").setIds({MECHANIC_SLOTH_TANTRUM}).setBoss(&boss_sloth),

	Mechanic().setName("got a bomb").setIds({MECHANIC_SLOTH_BOMB}).setFailIfHit(false).setFrequencyPlayer(6000).setBoss(&boss_sloth),

	Mechanic().setName("stood in bomb aoe").setIds({MECHANIC_SLOTH_BOMB_AOE}).setVerbosity(verbosity_chart).setBoss(&boss_sloth),

	Mechanic().setName("was hit by flame breath").setIds({MECHANIC_SLOTH_FLAME_BREATH}).setBoss(&boss_sloth),

	Mechanic().setName("was hit by shake").setIds({MECHANIC_SLOTH_SHAKE}).setBoss(&boss_sloth),

	Mechanic().setName("is fixated").setIds({MECHANIC_SLOTH_FIXATE}).setFailIfHit(false).setBoss(&boss_sloth),

//	Mechanic().setName("was hadoukened").setIds({MECHANIC_MATT_HADOUKEN_HUMAN,MECHANIC_MATT_HADOUKEN_ABOM}).setBoss(&boss_matti),

	Mechanic().setName("reflected shards").setIds({MECHANIC_MATT_SHARD_HUMAN,MECHANIC_MATT_SHARD_ABOM}).setTargetIsDst(false).setBoss(&boss_matti),

	Mechanic().setName("got a bomb").setIds({MECHANIC_MATT_BOMB}).setFailIfHit(false).setFrequencyPlayer(12000).setBoss(&boss_matti),

	Mechanic().setName("got a corruption").setIds({MECHANIC_MATT_CORRUPTION}).setFailIfHit(false).setBoss(&boss_matti),

	Mechanic().setName("is sacrificed").setIds({MECHANIC_MATT_SACRIFICE}).setFailIfHit(false).setBoss(&boss_matti),

	Mechanic().setName("is fixated").setIds({MECHANIC_KC_FIXATE}).setFailIfHit(false).setBoss(&boss_kc),
//	Mechanic().setName("is west fixated").setIds({MECHANIC_KC_FIXATE_WEST}).setFailIfHit(false).setBoss(&boss_kc),

//  Mechanic().setName("stood in the red half").setIds({MECHANIC_XERA_HALF}).setBoss(&boss_xera),

	Mechanic().setName("has magic").setIds({MECHANIC_XERA_MAGIC}).setFailIfHit(false).setValidIfDown(true).setValue(15000).setBoss(&boss_xera),

	Mechanic().setName("used magic").setIds({MECHANIC_XERA_MAGIC_BUFF}).setFailIfHit(false).setTargetIsDst(false).setFrequencyGlobal(12000).setValidIfDown(true).setBoss(&boss_xera).setSpecialRequirement(requirementOnSelf).setVerbosity(0),

	Mechanic().setName("triggered an orb").setIds({MECHANIC_XERA_ORB}).setBoss(&boss_xera),

	Mechanic().setName("stood in an orb aoe").setIds({MECHANIC_XERA_ORB_AOE}).setFrequencyPlayer(1000).setVerbosity(verbosity_chart).setBoss(&boss_xera),

	Mechanic().setName("was teleported").setIds({MECHANIC_XERA_PORT}).setVerbosity(verbosity_chart).setBoss(&boss_xera),

	Mechanic().setName("was teleported").setIds({MECHANIC_CARIN_TELEPORT}).setBoss(&boss_cairn),

	Mechanic().setName("was slapped").setIds({MECHANIC_CARIN_SWEEP}).setBoss(&boss_cairn).setIsInterupt(true),

//  Mechanic().setName("reflected shards").setIds({MECHANIC_CARIN_SHARD}).setTargetIsDst(false).setBoss(&boss_cairn),

	Mechanic().setName("missed a green circle").setIds({MECHANIC_CARIN_GREEN_A,MECHANIC_CARIN_GREEN_B,MECHANIC_CARIN_GREEN_C,MECHANIC_CARIN_GREEN_D,MECHANIC_CARIN_GREEN_E,MECHANIC_CARIN_GREEN_F}).setIsInterupt(true).setBoss(&boss_cairn),

	Mechanic().setName("was shockwaved").setIds({MECHANIC_SAM_SHOCKWAVE}).setIsInterupt(true).setBoss(&boss_sam),

	Mechanic().setName("was horizontally slapped").setIds({MECHANIC_SAM_SLAP_HORIZONTAL}).setIsInterupt(true).setBoss(&boss_sam),

	Mechanic().setName("was vertically smacked").setIds({MECHANIC_SAM_SLAP_VERTICAL}).setIsInterupt(true).setBoss(&boss_sam),

	Mechanic().setName("is fixated").setIds({MECHANIC_SAM_FIXATE_SAM}).setFailIfHit(false).setBoss(&boss_sam),

	Mechanic().setName("has big green").setIds({MECHANIC_SAM_GREEN_BIG}).setFailIfHit(false).setBoss(&boss_sam),
	Mechanic().setName("has small green").setIds({MECHANIC_SAM_GREEN_SMALL}).setFailIfHit(false).setBoss(&boss_sam),

	Mechanic().setName("touched an oil").setIds({MECHANIC_DEIMOS_OIL}).setFrequencyPlayer(5000).setBoss(&boss_deimos).setSpecialRequirement(requirementDeimosOil),

	Mechanic().setName("was smashed").setIds({MECHANIC_DEIMOS_SMASH,MECHANIC_DEIMOS_SMASH_INITIAL,MECHANIC_DEIMOS_SMASH_END_A,MECHANIC_DEIMOS_SMASH_END_B}).setBoss(&boss_deimos),

	Mechanic().setName("closed a tear").setIds({MECHANIC_DEIMOS_TEAR}).setFailIfHit(false).setBoss(&boss_deimos),

	Mechanic().setName("has the teleport").setIds({MECHANIC_DEIMOS_PORT_BUFF}).setValue(6500).setBoss(&boss_deimos),

	Mechanic().setName("was teleported").setIds({MECHANIC_DEIMOS_PORT}).setVerbosity(verbosity_chart).setBoss(&boss_deimos),

	Mechanic().setName("stood in inner ring").setIds({MECHANIC_HORROR_DONUT_INNER}).setVerbosity(verbosity_chart).setBoss(&boss_sh),

	Mechanic().setName("stood in outer ring").setIds({MECHANIC_HORROR_DONUT_OUTER}).setVerbosity(verbosity_chart).setBoss(&boss_sh),

	Mechanic().setName("stood in torment aoe").setIds({MECHANIC_HORROR_GOLEM_AOE}).setBoss(&boss_sh),

	Mechanic().setName("stood in pie slice").setIds({MECHANIC_HORROR_PIE_4_A,MECHANIC_HORROR_PIE_4_B}).setVerbosity(verbosity_chart).setBoss(&boss_sh),

	Mechanic().setName("touched a scythe").setIds({MECHANIC_HORROR_SCYTHE}).setBoss(&boss_sh),

	Mechanic().setName("took fixate").setIds({MECHANIC_HORROR_FIXATE}).setFailIfHit(false).setVerbosity(verbosity_chart).setBoss(&boss_sh),

	Mechanic().setName("was debuffed").setIds({MECHANIC_HORROR_DEBUFF}).setFailIfHit(false).setVerbosity(verbosity_chart).setBoss(&boss_sh),

	Mechanic().setName("was puked on").setIds({MECHANIC_EATER_PUKE}).setFrequencyPlayer(3000).setVerbosity(verbosity_chart).setBoss(&boss_soul_eater),

	Mechanic().setName("stood in web").setIds({MECHANIC_EATER_WEB}).setFrequencyPlayer(3000).setVerbosity(verbosity_chart).setBoss(&boss_soul_eater),

	Mechanic().setName("got an orb").setIds({MECHANIC_EATER_ORB}).setFrequencyPlayer(ms_per_tick).setBoss(&boss_soul_eater),

	Mechanic().setName("threw an orb").setNameInternal("Reclaimed Energy").setIds({47942}).setTargetIsDst(false).setIsActivation(ACTV_NORMAL).setBoss(&boss_soul_eater),

	Mechanic().setName("touched a messenger").setIds({MECHANIC_DHUUM_GOLEM}).setBoss(&boss_dhuum),

	Mechanic().setName("is shackled").setIds({MECHANIC_DHUUM_SHACKLE}).setFailIfHit(false).setTargetIsDst(false).setBoss(&boss_dhuum),

	Mechanic().setName("is shackled").setIds({MECHANIC_DHUUM_SHACKLE}).setFailIfHit(false).setBoss(&boss_dhuum),

//	Mechanic().setName("popped shackles").setIds({MECHANIC_DHUUM_SHACKLE}).setFailIfHit(false).setIsBuffremove(CBTB_MANUAL).setTargetIsDst(false).setSpecialValue(valueDhuumShackles).setBoss(&boss_dhuum),
//	Mechanic().setName("popped shackles").setIds({MECHANIC_DHUUM_SHACKLE}).setFailIfHit(false).setIsBuffremove(CBTB_MANUAL).setSpecialValue(valueDhuumShackles).setBoss(&boss_dhuum),

	Mechanic().setName("has affliction").setIds({MECHANIC_DHUUM_AFFLICTION}).setFrequencyPlayer(13000 + ms_per_tick).setFailIfHit(false).setValidIfDown(true).setBoss(&boss_dhuum),

	Mechanic().setName("stood in a crack").setIds({MECHANIC_DHUUM_CRACK}).setBoss(&boss_dhuum),

	Mechanic().setName("stood in a poison mark").setIds({MECHANIC_DHUUM_MARK}).setVerbosity(verbosity_chart).setBoss(&boss_dhuum),

	Mechanic().setName("was sucked center").setIds({MECHANIC_DHUUM_SUCK_AOE}).setBoss(&boss_dhuum),

	Mechanic().setName("stood in dip aoe").setIds({MECHANIC_DHUUM_TELEPORT_AOE}).setBoss(&boss_dhuum),

//	Mechanic().setName("died on green").setIds({MECHANIC_DHUUM_GREEN_TIMER}).setIsBuffremove(CBTB_MANUAL).setOverstackValue(0).setBoss(&boss_dhuum),

	Mechanic().setName("was snatched").setIds({MECHANIC_DHUUM_SNATCH}).setSpecialRequirement(requirementDhuumSnatch).setBoss(&boss_dhuum),

//	Mechanic().setName("canceled button channel").setIds({MECHANIC_DHUUM_BUTTON_CHANNEL}).setIsActivation(ACTV_CANCEL_CANCEL).setBoss(&boss_dhuum),

	Mechanic().setName("stood in cone").setIds({MECHANIC_DHUUM_CONE}).setBoss(&boss_dhuum),

	Mechanic().setName("was squashed").setIds({MECHANIC_AMAL_SQUASH}).setIsInterupt(true).setBoss(&boss_ca),

	Mechanic().setName("was shockwaved").setIds({MECHANIC_LARGOS_SHOCKWAVE}).setBoss(&boss_largos),

	Mechanic().setName("was waterlogged").setIds({MECHANIC_LARGOS_WATERLOGGED}).setVerbosity(verbosity_chart).setValidIfDown(true).setFrequencyPlayer(1).setBoss(&boss_largos),

	Mechanic().setName("was bubbled").setIds({MECHANIC_LARGOS_BUBBLE}).setBoss(&boss_largos),

	Mechanic().setName("has a tidal pool").setIds({MECHANIC_LARGOS_TIDAL_POOL}).setFailIfHit(false).setBoss(&boss_largos),

	Mechanic().setName("stood in geyser").setIds({MECHANIC_LARGOS_GEYSER}).setBoss(&boss_largos),

	Mechanic().setName("was dashed over").setIds({MECHANIC_LARGOS_DASH}).setBoss(&boss_largos),

	Mechanic().setName("had boons stolen").setIds({MECHANIC_LARGOS_BOON_RIP}).setBoss(&boss_largos),

	Mechanic().setName("stood in whirlpool").setIds({MECHANIC_LARGOS_WHIRLPOOL}).setBoss(&boss_largos),

	Mechanic().setName("was shockwaved").setIds({MECHANIC_QADIM_SHOCKWAVE_A,MECHANIC_QADIM_SHOCKWAVE_B}).setBoss(&boss_qadim),

	Mechanic().setName("stood in arcing fire").setIds({MECHANIC_QADIM_ARCING_FIRE_A,MECHANIC_QADIM_ARCING_FIRE_B,MECHANIC_QADIM_ARCING_FIRE_C}).setVerbosity(verbosity_chart).setBoss(&boss_qadim),

//	Mechanic().setName("stood in giant fireball").setIds({MECHANIC_QADIM_BOUNCING_FIREBALL_BIG_A,MECHANIC_QADIM_BOUNCING_FIREBALL_BIG_B,MECHANIC_QADIM_BOUNCING_FIREBALL_BIG_C}).setBoss(&boss_qadim),

	Mechanic().setName("was teleported").setIds({MECHANIC_QADIM_TELEPORT}).setBoss(&boss_qadim).setValidIfDown(true),

	Mechanic().setName("stood in hitbox").setNameInternal("Sea of Flame").setIds({52461}).setBoss(&boss_qadim),

	Mechanic().setName("got a flux bomb").setIds({MECHANIC_FOTM_FLUX_BOMB}).setFailIfHit(false).setBoss(&boss_fotm_generic),
	
//	Mechanic().setName("vomited on someone").setIds({MECHANIC_NIGHTMARE_VOMIT}).setTargetIsDst(false).setBoss(&boss_fotm_generic),

	Mechanic().setName("was hit by whirl").setIds({MECHANIC_MAMA_WHIRL,MECHANIC_MAMA_WHIRL_NORMAL}).setBoss(&boss_mama),

	Mechanic().setName("was knocked").setIds({MECHANIC_MAMA_KNOCK}).setBoss(&boss_mama),

	Mechanic().setName("was leaped on").setIds({MECHANIC_MAMA_LEAP}).setBoss(&boss_mama),

	Mechanic().setName("stood in acid").setIds({MECHANIC_MAMA_ACID}).setBoss(&boss_mama),

	Mechanic().setName("was smashed by a knight").setIds({MECHANIC_MAMA_KNIGHT_SMASH}).setBoss(&boss_mama),

	Mechanic().setName("stood in acid").setIds({MECHANIC_SIAX_ACID}).setBoss(&boss_siax),

	Mechanic().setName("was ran over").setIds({MECHANIC_ENSOLYSS_LUNGE}).setBoss(&boss_ensolyss),

	Mechanic().setName("was smashed").setIds({MECHANIC_ENSOLYSS_SMASH}).setBoss(&boss_ensolyss),

	Mechanic().setName("stood in a pie slice").setIds({MECHANIC_ARKK_PIE_A,MECHANIC_ARKK_PIE_B,MECHANIC_ARKK_PIE_C}).setBoss(&boss_arkk),

//  Mechanic().setName("was feared").setIds({MECHANIC_ARKK_FEAR}),

	Mechanic().setName("was smashed").setIds({MECHANIC_ARKK_OVERHEAD_SMASH}).setBoss(&boss_arkk),

	Mechanic().setName("has a bomb").setIds({MECHANIC_ARKK_BOMB}).setFailIfHit(false).setBoss(&boss_arkk),//TODO Add BOSS_ARTSARIIV_ID and make boss id a vector

//	Mechanic().setName("didn't block the goop").setIds({MECHANIC_ARKK_GOOP}).setBoss(&boss_arkk).setCanEvade(false),
};
