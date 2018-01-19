#include "mechanics.h"

bool have_added_line_break = true;
uint64_t last_mechanic_time = 0;
uint64_t line_break_frequency = 5000;
bool has_logged_mechanic = false;

mechanic::mechanic()
{
    name = "";
    frequency_player = 2000;
    frequency_global = 0;
    last_hit_time = 0;
    is_interupt = false;
    is_multihit = true;
    target_is_dst = true;
    fail_if_hit = true;
    is_buffremove = 0;
    valid_if_down = false;
    special_requirement = default_requirement;
}

bool mechanic::is_valid_hit(cbtevent* ev, ag* src, ag* dst)
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

        if(ev->is_buffremove != is_buffremove)
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
            current_player->mechanic_receive(name,ids[0],fail_if_hit);

            current_player->set_last_mechanic(ev->skillid);

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

mechanic vg_teleport = mechanic()
.set_name("was teleported")
.set_ids({MECHANIC_VG_TELEPORT_RAINBOW,MECHANIC_VG_TELEPORT_GREEN});

mechanic vg_green = mechanic()
.set_name("stood in the green circle")
.set_ids({MECHANIC_VG_GREEN_A,MECHANIC_VG_GREEN_B,MECHANIC_VG_GREEN_C,MECHANIC_VG_GREEN_D})
.set_fail_if_hit(false);

mechanic gors_slam = mechanic()
.set_name("was slammed")
.set_ids({MECHANIC_GORS_SLAM})
.set_is_interupt(true);

mechanic gors_egg = mechanic()
.set_name("was egged")
.set_ids({MECHANIC_GORS_EGG});

mechanic sab_sapper_bomb = mechanic()
.set_name("got a sapper bomb")
.set_ids({MECHANIC_SAB_SAPPER_BOMB})
.set_fail_if_hit(false)
.set_valid_if_down(true);

mechanic sab_time_bomb = mechanic()
.set_name("got a time bomb")
.set_ids({MECHANIC_SAB_TIME_BOMB})
.set_fail_if_hit(false)
.set_valid_if_down(true);

mechanic sab_cannon = mechanic()
.set_name("stood in cannon fire")
.set_ids({MECHANIC_SAB_CANNON});

mechanic sab_flamewall = mechanic()
.set_name("touched the flame wall")
.set_ids({MECHANIC_SAB_FLAMEWALL});

mechanic sloth_tantrum = mechanic()
.set_name("was hit with tantrum")
.set_ids({MECHANIC_SLOTH_TANTRUM});

mechanic sloth_bomb = mechanic()
.set_name("got a bomb")
.set_ids({MECHANIC_SLOTH_BOMB})
.set_fail_if_hit(false)
.set_frequency_player(6000);

mechanic sloth_bomb_aoe = mechanic()
.set_name("stood in a bomb aoe")
.set_ids({MECHANIC_SLOTH_BOMB_AOE});

mechanic sloth_flame = mechanic()
.set_name("was hit by flame breath")
.set_ids({MECHANIC_SLOTH_FLAME_BREATH});

mechanic sloth_shake = mechanic()
.set_name("was hit by shake")
.set_ids({MECHANIC_SLOTH_SHAKE});

mechanic sloth_fixate = mechanic()
.set_name("is fixated")
.set_ids({MECHANIC_SLOTH_FIXATE})
.set_fail_if_hit(false);

mechanic matt_hadouken = mechanic()
.set_name("was hadoukened")
.set_ids({MECHANIC_MATT_HADOUKEN_HUMAN,MECHANIC_MATT_HADOUKEN_ABOM});

mechanic matt_shard_reflect = mechanic()
.set_name("reflected shards")
.set_ids({MECHANIC_MATT_SHARD_HUMAN,MECHANIC_MATT_SHARD_ABOM})
.set_target_is_dst(false);

mechanic matt_bomb = mechanic()
.set_name("got a bomb")
.set_ids({MECHANIC_MATT_BOMB})
.set_fail_if_hit(false)
.set_frequency_player(12000);

mechanic matt_corruption = mechanic()
.set_name("got a corruption")
.set_ids({MECHANIC_MATT_CORRUPTION})
.set_fail_if_hit(false);

mechanic matt_sacrifice = mechanic()
.set_name("is sacrificed")
.set_ids({MECHANIC_MATT_SACRIFICE})
.set_fail_if_hit(false);

mechanic kc_fixate_a = mechanic()
.set_name("is fixate A")
.set_ids({MECHANIC_KC_FIXATE_A})
.set_fail_if_hit(false);

mechanic kc_fixate_b = mechanic()
.set_name("is fixate B")
.set_ids({MECHANIC_KC_FIXATE_B})
.set_fail_if_hit(false);

mechanic xera_half = mechanic()
.set_name("stood in the red half")
.set_ids({MECHANIC_XERA_HALF});

mechanic xera_magic = mechanic()
.set_name("has magic")
.set_ids({MECHANIC_XERA_MAGIC})
.set_fail_if_hit(false)
.set_target_is_dst(false)
.set_frequency_global(12000)
.set_valid_if_down(true);

mechanic xera_orb = mechanic()
.set_name("touched an orb")
.set_ids({MECHANIC_XERA_ORB});


mechanic xera_orb_aoe = mechanic()
.set_name("stood in an orb aoe")
.set_ids({MECHANIC_XERA_ORB_AOE})
.set_frequency_player(1000);

mechanic carin_teleport = mechanic()
.set_name("was teleported")
.set_ids({MECHANIC_CARIN_TELEPORT});

mechanic carin_shard_reflect = mechanic()
.set_name("reflected shards")
.set_ids({MECHANIC_CARIN_SHARD})
.set_target_is_dst(false);

mechanic carin_green = mechanic()
.set_name("missed a green circle")
.set_ids({MECHANIC_CARIN_GREEN})
.set_is_interupt(true);

mechanic sam_shockwave = mechanic()
.set_name("was hit by shockwave")
.set_ids({MECHANIC_SAM_SHOCKWAVE})
.set_is_interupt(true);

mechanic sam_slap_horizontal = mechanic()
.set_name("was horizontally slapped")
.set_ids({MECHANIC_SAM_SLAP_HORIZONTAL})
.set_is_interupt(true);

mechanic sam_slap_vertical = mechanic()
.set_name("was vertically smacked")
.set_ids({MECHANIC_SAM_SLAP_VERTICAL})
.set_is_interupt(true);

mechanic deimos_oil = mechanic()
.set_name("touched an oil")
.set_ids({MECHANIC_DEIMOS_OIL})
.set_frequency_global(8000);

mechanic deimos_smash = mechanic()
.set_name("was hit by smash")
.set_ids({MECHANIC_DEIMOS_SMASH,MECHANIC_DEIMOS_SMASH_INITIAL})
.set_is_interupt(true);

mechanic horror_donut_inner = mechanic()
.set_name("stood in inner donut")
.set_ids({MECHANIC_HORROR_DONUT_INNER});

mechanic horror_donut_outer = mechanic()
.set_name("stood in outer donut")
.set_ids({MECHANIC_HORROR_DONUT_OUTER});

mechanic horror_golem_aoe = mechanic()
.set_name("stood in torment aoe")
.set_ids({MECHANIC_HORROR_GOLEM_AOE});

mechanic horror_pie = mechanic()
.set_name("stood in pie slice")
.set_ids({MECHANIC_HORROR_PIE_4_A,MECHANIC_HORROR_PIE_4_B});

mechanic horror_scythe = mechanic()
.set_name("touched a scythe")
.set_ids({MECHANIC_HORROR_SCYTHE});

mechanic horror_agro = mechanic()
.set_name("is fixated")
.set_ids({MECHANIC_HORROR_FIXATE})
.set_fail_if_hit(false);

mechanic dhuum_golem = mechanic()
.set_name("touched a messenger")
.set_ids({MECHANIC_DHUUM_GOLEM});

mechanic dhuum_shackle_src = mechanic()
.set_name("is shackled")
.set_ids({MECHANIC_DHUUM_SHACKLE})
.set_fail_if_hit(false)
.set_target_is_dst(false);

mechanic dhuum_shackle_dst = mechanic()
.set_name("is shackled")
.set_ids({MECHANIC_DHUUM_SHACKLE})
.set_fail_if_hit(false);

mechanic dhuum_affliction = mechanic()
.set_name("has affliction")
.set_ids({MECHANIC_DHUUM_AFFLICTION})
.set_frequency_player(13000 + ms_per_tick)
.set_fail_if_hit(false);

mechanic dhuum_crack = mechanic()
.set_name("stood in a crack")
.set_ids({MECHANIC_DHUUM_CRACK});

mechanic dhuum_mark = mechanic()
.set_name("stood in a mark")
.set_ids({MECHANIC_DHUUM_MARK});

mechanic dhuum_suck_aoe = mechanic()
.set_name("touched the center during suction")
.set_ids({MECHANIC_DHUUM_SUCK_AOE});

mechanic dhuum_teleport_aoe = mechanic()
.set_name("stood in a teleport aoe")
.set_ids({MECHANIC_DHUUM_TELEPORT_AOE});

mechanic dhuum_snatch = mechanic()
.set_name("was snatched")
.set_ids({MECHANIC_DHUUM_SNATCH})
.set_frequency_player(26000);//time found as 100% hp to 0 on minstrel/monk druid with regen

mechanic nightmare_vomit = mechanic()
.set_name("vomited on someone")
.set_ids({MECHANIC_NIGHTMARE_VOMIT})
.set_target_is_dst(false);

mechanic mama_wirl = mechanic()
.set_name("was hit by wirl")
.set_ids({MECHANIC_MAMA_WIRL});

mechanic mama_knock = mechanic()
.set_name("was knocked")
.set_ids({MECHANIC_MAMA_KNOCK});

mechanic mama_leap = mechanic()
.set_name("was leaped on")
.set_ids({MECHANIC_MAMA_LEAP});

mechanic mama_acid = mechanic()
.set_name("stood in acid")
.set_ids({MECHANIC_MAMA_ACID});

mechanic mama_knight_smash = mechanic()
.set_name("was smashed by a knight")
.set_ids({MECHANIC_MAMA_KNIGHT_SMASH});

mechanic siax_acid = mechanic()
.set_name("stood in acid")
.set_ids({MECHANIC_SIAX_ACID});

mechanic ensolyss_lunge = mechanic()
.set_name("was ran over")
.set_ids({MECHANIC_ENSOLYSS_LUNGE});

mechanic ensolyss_smash = mechanic()
.set_name("was smashed")
.set_ids({MECHANIC_ENSOLYSS_SMASH});

mechanic arkk_pie = mechanic()
.set_name("stood in a pie slice")
.set_ids({MECHANIC_ARKK_PIE_A,MECHANIC_ARKK_PIE_B,MECHANIC_ARKK_PIE_C});

mechanic arkk_fear = mechanic()
.set_name("was feared")
.set_ids({MECHANIC_ARKK_FEAR});

mechanic arkk_overhead_smash = mechanic()
.set_name("was smashed")
.set_ids({MECHANIC_ARKK_OVERHEAD_SMASH});

mechanic arkk_bomb = mechanic()
.set_name("has a bomb")
.set_ids({MECHANIC_ARKK_BOMB})
.set_fail_if_hit(false);



bool special_requirement_conjure(const mechanic &current_mechanic, cbtevent* ev, ag* src, ag* dst, Player* current_player)
{
    return dst->prof != 6;//not elementalist
}

mechanic conjure_ice_bow = mechanic()
.set_name("picked up an ice bow")
.set_ids({CONJURE_ICE_BOW_BUFF})
.set_fail_if_hit(false)
.set_special_requirement(special_requirement_conjure);

mechanic conjure_lightning_hammer = mechanic()
.set_name("picked up a lightning hammer")
.set_ids({CONJURE_LIGHTNING_HAMMER_BUFF})
.set_fail_if_hit(false)
.set_special_requirement(special_requirement_conjure);

mechanic conjure_flame_axe = mechanic()
.set_name("picked up a flame axe")
.set_ids({CONJURE_FLAME_AXE_BUFF})
.set_fail_if_hit(false)
.set_special_requirement(special_requirement_conjure);

mechanic conjure_earth_shield = mechanic()
.set_name("picked up an earth shield")
.set_ids({CONJURE_EARTH_SHIELD_BUFF})
.set_fail_if_hit(false)
.set_special_requirement(special_requirement_conjure);

mechanic conjure_fire_gs = mechanic()
.set_name("picked up an FGS")
.set_ids({CONJURE_FIRE_GS_BUFF})
.set_fail_if_hit(false)
.set_special_requirement(special_requirement_conjure);

std::vector <mechanic> mechanics =
{
    vg_teleport,
//    vg_green,
    gors_slam,
    gors_egg,
    sab_sapper_bomb,
    sab_time_bomb,
    sab_cannon,
    sab_flamewall,
    sloth_tantrum,
    sloth_bomb,
    sloth_bomb_aoe,
    sloth_flame,
    sloth_shake,
    sloth_fixate,
    matt_hadouken,
    matt_shard_reflect,
    matt_bomb,
    matt_corruption,
    matt_sacrifice,
    kc_fixate_a,
    kc_fixate_b,
//    xera_half,
    xera_orb,
    xera_orb_aoe,
    xera_magic,
    carin_teleport,
//    carin_shard_reflect,
    carin_green,
    sam_shockwave,
    sam_slap_horizontal,
    sam_slap_vertical,
    deimos_oil,
    deimos_smash,
    horror_donut_inner,
    horror_donut_outer,
    horror_golem_aoe,
    horror_pie,
    horror_scythe,
    horror_agro,
    dhuum_golem,
    dhuum_affliction,
    dhuum_shackle_src,
    dhuum_shackle_dst,
    dhuum_crack,
//    dhuum_mark,
    dhuum_suck_aoe,
    dhuum_teleport_aoe,
    dhuum_snatch,
    nightmare_vomit,
    mama_wirl,
    mama_knock,
    mama_leap,
    mama_acid,
    mama_knight_smash,
    siax_acid,
    ensolyss_lunge,
    ensolyss_smash,
    arkk_pie,
//    arkk_fear,
    arkk_overhead_smash,
    arkk_bomb,

#if 0//disable conjure detection due to potential toxicity
    conjure_ice_bow,
    conjure_lightning_hammer,
    conjure_flame_axe,
    conjure_earth_shield,
    conjure_fire_gs
#endif // 0
};
