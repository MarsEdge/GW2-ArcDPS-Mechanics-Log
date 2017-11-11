#include "mechanics.h"

bool have_added_line_break = true;
uint64_t last_mechanic_time = 0;
uint64_t line_break_frequency = 5000;
bool has_logged_mechanic = false;

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
    matt_hadouken,
    matt_shard_reflect,
    matt_bomb,
//    xera_half,
    xera_orb,
    xera_orb_aoe,
    xera_magic,
    carin_teleport,
    carin_shard_reflect,
    carin_green,
    sam_shockwave,
    sam_slap_horizontal,
    sam_slap_vertical,
    deimos_oil,
    deimos_smash
};

mechanic::mechanic()
{
    name = "";
    frequency_player = 2000;
    frequency_global = 0;
    last_hit_time = 0;
    is_interupt = false;
    target_is_dst = true;
    fail_if_hit = true;
}

bool mechanic::is_valid_hit(cbtevent* &ev, ag* &src, ag* &dst)
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

        if(target_is_dst && is_player(dst))
        {
            current_player=get_player(ev->dst_instid);
        }
        else if(!target_is_dst && is_player(src))
        {
            current_player=get_player(ev->src_instid);
        }

        if(current_player
           && (!is_multihit || ev->time > (current_player->last_hit_time+this->frequency_player))
           && (!is_interupt || current_player->last_stab_time < ev->time))
        {
            current_player->last_hit_time=ev->time;
            last_hit_time = ev->time;
            if(fail_if_hit)
            {
                current_player->mechanics_failed++;
            }
            current_player->last_machanic=ev->skillid;

            last_mechanic_time = ev->time;
            have_added_line_break = false;
            has_logged_mechanic = true;

            return true;
        }
    }
    return false;
}

vg_teleport::vg_teleport()
{
    name="was teleported"; //name of mechanic
    ids.push_back(MECHANIC_VG_TELEPORT_RAINBOW);
    ids.push_back(MECHANIC_VG_TELEPORT_GREEN);
};

vg_green::vg_green()
{
    name="stood in the green circle"; //name of mechanic
    ids.push_back(MECHANIC_VG_GREEN_A);
    ids.push_back(MECHANIC_VG_GREEN_B);
    ids.push_back(MECHANIC_VG_GREEN_C);
    ids.push_back(MECHANIC_VG_GREEN_D);
    fail_if_hit = false;
}

gors_slam::gors_slam()
{
    name="was slammed"; //name of mechanic
    ids.push_back(MECHANIC_GORS_SLAM); //skill id;
    is_interupt=true;
}

gors_egg::gors_egg()
{
    name="was egged"; //name of mechanic
    ids.push_back(MECHANIC_GORS_EGG); //skill id;
}
sab_sapper_bomb::sab_sapper_bomb()
{
    name="got a sapper bomb"; //name of mechanic
    ids.push_back(MECHANIC_SAB_SAPPER_BOMB); //skill id;
}

sab_time_bomb::sab_time_bomb()
{
    name="got a time bomb"; //name of mechanic
    ids.push_back(MECHANIC_SAB_TIME_BOMB); //skill id;
}

sab_cannon::sab_cannon()
{
    name="stood in cannon fire"; //name of mechanic
    ids.push_back(MECHANIC_SAB_CANNON); //skill id;
}

sab_flamewall::sab_flamewall()
{
    name="touched the flame wall"; //name of mechanic
    ids.push_back(MECHANIC_SAB_FLAMEWALL); //skill id;
}

sloth_tantrum::sloth_tantrum()
{
    name="was hit with tantrum"; //name of mechanic
    ids.push_back(MECHANIC_SLOTH_TANTRUM); //skill id;
}

sloth_bomb::sloth_bomb()
{
    name="got a bomb"; //name of mechanic
    ids.push_back(MECHANIC_SLOTH_BOMB); //skill id;
    fail_if_hit = false;
    frequency_player = 6000;
}

sloth_bomb_aoe::sloth_bomb_aoe()
{
    name="stood in a bomb aoe"; //name of mechanic
    ids.push_back(MECHANIC_SLOTH_BOMB_AOE); //skill id;
}

sloth_flame::sloth_flame()
{
    name="was hit by flame breath"; //name of mechanic
    ids.push_back(MECHANIC_SLOTH_FLAME_BREATH); //skill id;
}

sloth_shake::sloth_shake()
{
    name="was hit by shake"; //name of mechanic
    ids.push_back(MECHANIC_SLOTH_SHAKE); //skill id;
}

matt_hadouken::matt_hadouken()
{
    name="was hadoukened"; //name of mechanic
    ids.push_back(MECHANIC_MATT_HADOUKEN_HUMAN); //skill id;
    ids.push_back(MECHANIC_MATT_HADOUKEN_ABOM); //skill id;
}

matt_shard_reflect::matt_shard_reflect()
{
    name="reflected shards"; //name of mechanic
    ids.push_back(MECHANIC_MATT_SHARD_HUMAN); //skill id;
    ids.push_back(MECHANIC_MATT_SHARD_ABOM); //skill id;
    target_is_dst = false;
}

matt_bomb::matt_bomb()
{
    name="got a bomb"; //name of mechanic
    ids.push_back(MECHANIC_MATT_BOMB); //skill id;
    frequency_player = 12000;
}

xera_half::xera_half()
{
    name="stood in the red half"; //name of mechanic
    ids.push_back(MECHANIC_XERA_HALF); //skill id;
}

xera_magic::xera_magic()
{
    name="has magic"; //name of mechanic
    ids.push_back(MECHANIC_XERA_MAGIC); //skill id;
    fail_if_hit = false;
    target_is_dst = false;
    frequency_global = 12000;
}

xera_orb::xera_orb()
{
    name="touched an orb"; //name of mechanic
    ids.push_back(MECHANIC_XERA_ORB); //skill id;
}

xera_orb_aoe::xera_orb_aoe()
{
    name="stood in an orb aoe"; //name of mechanic
    ids.push_back(MECHANIC_XERA_ORB_AOE); //skill id;
}

carin_teleport::carin_teleport()
{
    name="was teleported"; //name of mechanic
    ids.push_back(MECHANIC_CARIN_TELEPORT); //skill id;
}

carin_shard_reflect::carin_shard_reflect()
{
    name="reflected shards"; //name of mechanic
    ids.push_back(MECHANIC_CARIN_SHARD); //skill id;
    target_is_dst = false;
}

carin_green::carin_green()
{
    name="missed a green circle"; //name of mechanic
    ids.push_back(MECHANIC_CARIN_GREEN); //skill id;
    is_interupt = true;
}

sam_shockwave::sam_shockwave()
{
    name="was hit by shockwave"; //name of mechanic
    ids.push_back(MECHANIC_SAM_SHOCKWAVE); //skill id;
    is_interupt=true;
}

sam_slap_horizontal::sam_slap_horizontal()
{
    name="was horizontally slapped"; //name of mechanic
    ids.push_back(MECHANIC_SAM_SLAP_HORIZONTAL); //skill id;
    is_interupt=true;
}

sam_slap_vertical::sam_slap_vertical()
{
    name="was vertically smacked"; //name of mechanic
    ids.push_back(MECHANIC_SAM_SLAP_VERTICAL); //skill id;
    is_interupt=true;
}

deimos_oil::deimos_oil()
{
    name="touched an oil"; //name of mechanic
    ids.push_back(MECHANIC_DEIMOS_OIL); //skill id;
    frequency_global = 8000;
}

deimos_smash::deimos_smash()
{
    name="was hit by smash"; //name of mechanic
    ids.push_back(MECHANIC_DEIMOS_SMASH); //skill id;
    ids.push_back(MECHANIC_DEIMOS_SMASH_INITIAL);
    is_interupt=true;
}
