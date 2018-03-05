#include "mechanics.h"

bool have_added_line_break = true;
uint64_t last_mechanic_time = 0;
uint64_t line_break_frequency = 5000;
bool has_logged_mechanic = false;

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
    is_buffremove = 0;
	overstack_value = -1;
    valid_if_down = false;
    special_requirement = default_requirement;
}

bool mechanic::is_valid_hit(cbtevent* ev, ag* src, ag* dst, game_state* gs)
{
    uint16_t index = 0;
    bool correct_id = false;
    Player* current_player = nullptr;

    for(index=0;index<ids.size();index++)
    {
        if(ev->skillid==this->ids[index])
        {
            correct_id = true;
            break;
        }
    }

    if(correct_id)//correct skill id
    {
        if(frequency_global != 0
           && ev->time < last_hit_time+frequency_global-ms_per_tick)
        {
            return false;
        }

        if(boss_id
           && gs->boss_found
           && !gs->boss_data.has_id(boss_id))
        {
            return false;
        }

        if(ev->is_buffremove != is_buffremove)
        {
            return false;
        }

		if (is_buffremove
			&& overstack_value >= 0
			&& overstack_value != ev->overstack_value)
		{
			return false;
		}

        if(target_is_dst && is_player(dst))
        {
            current_player=get_player(dst);
        }
        else if(!target_is_dst && is_player(src))
        {
            current_player=get_player(src);
        }

        if(current_player
           && (!is_multihit || ev->time >= (current_player->get_last_hit_time()+frequency_player))
           && (!current_player->is_downed || valid_if_down)
           && (!is_interupt || current_player->get_last_stab_time() <= ev->time)
           && special_requirement(*this,ev,src,dst,current_player))
        {
            current_player->set_last_hit_time(ev->time);
            last_hit_time = ev->time;
            current_player->mechanic_receive(name,ids[0],fail_if_hit, &gs->boss_data);

            last_mechanic_time = ev->time;
            have_added_line_break = false;
            has_logged_mechanic = true;

            return true;
        }
    }
    return false;
}

bool default_requirement(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
    return true;
}

bool special_requirement_conjure(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
    return dst->prof != 6;//not elementalist
}

bool special_requirement_dhuum_snatch(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
    if((current_player->get_last_hit_time() + current_mechanic.frequency_player) < ev->time)
    {
        current_player->set_last_hit_time(ev->time);
        if(current_player->get_last_mechanic() != MECHANIC_DHUUM_SNATCH)
        {
            return true;
        }
    }
    return false;
}

std::vector <mechanic> mechanics =
{
    mechanic().set_name("was teleported").set_ids({MECHANIC_VG_TELEPORT_RAINBOW,MECHANIC_VG_TELEPORT_GREEN}).set_boss_id(BOSS_VG_ID),

//  mechanic().set_name("stood in the green circle").set_ids({MECHANIC_VG_GREEN_A,MECHANIC_VG_GREEN_B,MECHANIC_VG_GREEN_C,MECHANIC_VG_GREEN_D}).set_fail_if_hit(false).set_boss_id(BOSS_GORS_ID),

	mechanic().set_name("was slammed").set_ids({MECHANIC_GORS_SLAM}).set_is_interupt(true).set_boss_id(BOSS_GORS_ID),

	mechanic().set_name("was egged").set_ids({MECHANIC_GORS_EGG}).set_boss_id(BOSS_GORS_ID),

	mechanic().set_name("touched an orb").set_ids({MECHANIC_GORS_ORB}).set_boss_id(BOSS_GORS_ID),

	mechanic().set_name("got a sapper bomb").set_ids({MECHANIC_SAB_SAPPER_BOMB}).set_fail_if_hit(false).set_valid_if_down(true).set_boss_id(BOSS_SAB_ID),

	mechanic().set_name("got a time bomb").set_ids({MECHANIC_SAB_TIME_BOMB}).set_fail_if_hit(false).set_valid_if_down(true).set_boss_id(BOSS_SAB_ID),

	mechanic().set_name("stood in cannon fire").set_ids({MECHANIC_SAB_CANNON}).set_boss_id(BOSS_SAB_ID),

	mechanic().set_name("touched the flame wall").set_ids({MECHANIC_SAB_FLAMEWALL}).set_boss_id(BOSS_SAB_ID),

	mechanic().set_name("was hit with tantrum").set_ids({MECHANIC_SLOTH_TANTRUM}).set_boss_id(BOSS_SLOTH_ID),

	mechanic().set_name("got a bomb").set_ids({MECHANIC_SLOTH_BOMB}).set_fail_if_hit(false).set_frequency_player(6000).set_boss_id(BOSS_SLOTH_ID),

	mechanic().set_name("stood in a bomb aoe").set_ids({MECHANIC_SLOTH_BOMB_AOE}).set_boss_id(BOSS_SLOTH_ID),

	mechanic().set_name("was hit by flame breath").set_ids({MECHANIC_SLOTH_FLAME_BREATH}).set_boss_id(BOSS_SLOTH_ID),

	mechanic().set_name("was hit by shake").set_ids({MECHANIC_SLOTH_SHAKE}).set_boss_id(BOSS_SLOTH_ID),

	mechanic().set_name("is fixated").set_ids({MECHANIC_SLOTH_FIXATE}).set_fail_if_hit(false).set_boss_id(BOSS_SLOTH_ID),

	mechanic().set_name("was hadoukened").set_ids({MECHANIC_MATT_HADOUKEN_HUMAN,MECHANIC_MATT_HADOUKEN_ABOM}).set_boss_id(BOSS_MATT_ID),

	mechanic().set_name("reflected shards").set_ids({MECHANIC_MATT_SHARD_HUMAN,MECHANIC_MATT_SHARD_ABOM}).set_target_is_dst(false).set_boss_id(BOSS_MATT_ID),

	mechanic().set_name("got a bomb").set_ids({MECHANIC_MATT_BOMB}).set_fail_if_hit(false).set_frequency_player(12000).set_boss_id(BOSS_MATT_ID),

	mechanic().set_name("got a corruption").set_ids({MECHANIC_MATT_CORRUPTION}).set_fail_if_hit(false).set_boss_id(BOSS_MATT_ID),

	mechanic().set_name("is sacrificed").set_ids({MECHANIC_MATT_SACRIFICE}).set_fail_if_hit(false).set_boss_id(BOSS_MATT_ID),

	mechanic().set_name("is fixated").set_ids({MECHANIC_KC_FIXATE_A,MECHANIC_KC_FIXATE_B}).set_fail_if_hit(false).set_boss_id(BOSS_KC_ID),

//  mechanic().set_name("stood in the red half").set_ids({MECHANIC_XERA_HALF}).set_boss_id(BOSS_XERA_ID_A),

	mechanic().set_name("has magic").set_ids({MECHANIC_XERA_MAGIC}).set_fail_if_hit(false).set_target_is_dst(false).set_frequency_global(12000).set_valid_if_down(true).set_boss_id(BOSS_XERA_ID_A),

	mechanic().set_name("touched an orb").set_ids({MECHANIC_XERA_ORB}).set_boss_id(BOSS_XERA_ID_A),

	mechanic().set_name("stood in an orb aoe").set_ids({MECHANIC_XERA_ORB_AOE}).set_frequency_player(1000).set_boss_id(BOSS_XERA_ID_A),

	mechanic().set_name("was teleported").set_ids({MECHANIC_CARIN_TELEPORT}).set_boss_id(BOSS_CAIRN_ID),

//  mechanic().set_name("reflected shards").set_ids({MECHANIC_CARIN_SHARD}).set_target_is_dst(false).set_boss_id(BOSS_CAIRN_ID),

	mechanic().set_name("missed a green circle").set_ids({MECHANIC_CARIN_GREEN_A,MECHANIC_CARIN_GREEN_B,MECHANIC_CARIN_GREEN_C,MECHANIC_CARIN_GREEN_D,MECHANIC_CARIN_GREEN_E,MECHANIC_CARIN_GREEN_F}).set_is_interupt(true).set_boss_id(BOSS_CAIRN_ID),

	mechanic().set_name("was hit by shockwave").set_ids({MECHANIC_SAM_SHOCKWAVE}).set_is_interupt(true).set_boss_id(BOSS_SAM_ID),

	mechanic().set_name("was horizontally slapped").set_ids({MECHANIC_SAM_SLAP_HORIZONTAL}).set_is_interupt(true).set_boss_id(BOSS_SAM_ID),

	mechanic().set_name("was vertically smacked").set_ids({MECHANIC_SAM_SLAP_VERTICAL}).set_is_interupt(true).set_boss_id(BOSS_SAM_ID),

	mechanic().set_name("is fixated").set_ids({MECHANIC_SAM_FIXATE_SAM}).set_fail_if_hit(false).set_boss_id(BOSS_SAM_ID),

	mechanic().set_name("touched an oil").set_ids({MECHANIC_DEIMOS_OIL}).set_frequency_global(8000).set_boss_id(BOSS_DEIMOS_ID),

	mechanic().set_name("was hit by smash").set_ids({MECHANIC_DEIMOS_SMASH,MECHANIC_DEIMOS_SMASH_INITIAL,MECHANIC_DEIMOS_SMASH_END_A,MECHANIC_DEIMOS_SMASH_END_B}).set_boss_id(BOSS_DEIMOS_ID),

	mechanic().set_name("closed a tear").set_ids({MECHANIC_DEIMOS_TEAR}).set_boss_id(BOSS_DEIMOS_ID),

	mechanic().set_name("stood in inner donut").set_ids({MECHANIC_HORROR_DONUT_INNER}).set_boss_id(BOSS_SH_ID),

	mechanic().set_name("stood in outer donut").set_ids({MECHANIC_HORROR_DONUT_OUTER}).set_boss_id(BOSS_SH_ID),

	mechanic().set_name("stood in torment aoe").set_ids({MECHANIC_HORROR_GOLEM_AOE}).set_boss_id(BOSS_SH_ID),

	mechanic().set_name("stood in pie slice").set_ids({MECHANIC_HORROR_PIE_4_A,MECHANIC_HORROR_PIE_4_B}).set_boss_id(BOSS_SH_ID),

	mechanic().set_name("touched a scythe").set_ids({MECHANIC_HORROR_SCYTHE}).set_boss_id(BOSS_SH_ID),

	mechanic().set_name("took fixate").set_ids({MECHANIC_HORROR_FIXATE}).set_fail_if_hit(false).set_boss_id(BOSS_SH_ID),

	mechanic().set_name("was debuffed").set_ids({MECHANIC_HORROR_DEBUFF}).set_fail_if_hit(false).set_boss_id(BOSS_SH_ID),

	mechanic().set_name("touched a messenger").set_ids({MECHANIC_DHUUM_GOLEM}).set_boss_id(BOSS_DHUUM_ID),

	mechanic().set_name("is shackled").set_ids({MECHANIC_DHUUM_SHACKLE}).set_fail_if_hit(false).set_target_is_dst(false).set_boss_id(BOSS_DHUUM_ID),

	mechanic().set_name("is shackled").set_ids({MECHANIC_DHUUM_SHACKLE}).set_fail_if_hit(false).set_boss_id(BOSS_DHUUM_ID),

	mechanic().set_name("has affliction").set_ids({MECHANIC_DHUUM_AFFLICTION}).set_frequency_player(13000 + ms_per_tick).set_fail_if_hit(false).set_valid_if_down(true).set_boss_id(BOSS_DHUUM_ID),

	mechanic().set_name("stood in a crack").set_ids({MECHANIC_DHUUM_CRACK}).set_boss_id(BOSS_DHUUM_ID),

//  mechanic().set_name("stood in a mark").set_ids({MECHANIC_DHUUM_MARK}).set_boss_id(BOSS_DHUUM_ID),

	mechanic().set_name("touched the center").set_ids({MECHANIC_DHUUM_SUCK_AOE}).set_boss_id(BOSS_DHUUM_ID),

	mechanic().set_name("stood in a teleport aoe").set_ids({MECHANIC_DHUUM_TELEPORT_AOE}).set_boss_id(BOSS_DHUUM_ID),

	mechanic().set_name("died on green").set_ids({MECHANIC_DHUUM_GREEN_TIMER}).set_is_buffremove(3).set_overstack_value(0),

	mechanic().set_name("was snatched").set_ids({MECHANIC_DHUUM_SNATCH}).set_special_requirement(special_requirement_dhuum_snatch).set_boss_id(BOSS_DHUUM_ID),

	mechanic().set_name("vomited on someone").set_ids({MECHANIC_NIGHTMARE_VOMIT}).set_target_is_dst(false),

	mechanic().set_name("was hit by wirl").set_ids({MECHANIC_MAMA_WIRL,MECHANIC_MAMA_WIRL_NORMAL}).set_boss_id(BOSS_MAMA_ID),

	mechanic().set_name("was knocked").set_ids({MECHANIC_MAMA_KNOCK}).set_boss_id(BOSS_MAMA_ID),

	mechanic().set_name("was leaped on").set_ids({MECHANIC_MAMA_LEAP}).set_boss_id(BOSS_MAMA_ID),

	mechanic().set_name("stood in acid").set_ids({MECHANIC_MAMA_ACID}).set_boss_id(BOSS_MAMA_ID),

	mechanic().set_name("was smashed by a knight").set_ids({MECHANIC_MAMA_KNIGHT_SMASH}).set_boss_id(BOSS_MAMA_ID),

	mechanic().set_name("stood in acid").set_ids({MECHANIC_SIAX_ACID}).set_boss_id(BOSS_SIAX_ID),

	mechanic().set_name("was ran over").set_ids({MECHANIC_ENSOLYSS_LUNGE}).set_boss_id(BOSS_ENSOLYSS_ID),

	mechanic().set_name("was smashed").set_ids({MECHANIC_ENSOLYSS_SMASH}).set_boss_id(BOSS_ENSOLYSS_ID),

	mechanic().set_name("stood in a pie slice").set_ids({MECHANIC_ARKK_PIE_A,MECHANIC_ARKK_PIE_B,MECHANIC_ARKK_PIE_C}).set_boss_id(BOSS_ARKK_ID),

//  mechanic().set_name("was feared").set_ids({MECHANIC_ARKK_FEAR}),

	mechanic().set_name("was smashed").set_ids({MECHANIC_ARKK_OVERHEAD_SMASH}).set_boss_id(BOSS_ARKK_ID),

	mechanic().set_name("has a bomb").set_ids({MECHANIC_ARKK_BOMB}).set_fail_if_hit(false).set_boss_id(BOSS_ARKK_ID),//TODO Add BOSS_ARTSARIIV_ID and make boss id a vector

#if 0//disable conjure detection due to potential toxicity
    mechanic().set_name("picked up an ice bow").set_ids({CONJURE_ICE_BOW_BUFF}).set_fail_if_hit(false).set_special_requirement(special_requirement_conjure),

	mechanic().set_name("picked up a lightning hammer").set_ids({CONJURE_LIGHTNING_HAMMER_BUFF}).set_fail_if_hit(false).set_special_requirement(special_requirement_conjure),

	mechanic().set_name("picked up a flame axe").set_ids({CONJURE_FLAME_AXE_BUFF}).set_fail_if_hit(false).set_special_requirement(special_requirement_conjure),

	mechanic().set_name("picked up an earth shield").set_ids({CONJURE_EARTH_SHIELD_BUFF}).set_fail_if_hit(false).set_special_requirement(special_requirement_conjure),

	mechanic().set_name("picked up an FGS").set_ids({CONJURE_FIRE_GS_BUFF}).set_fail_if_hit(false).set_special_requirement(special_requirement_conjure)
#endif // 0
};
