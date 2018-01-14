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
    matt_corruption,
    matt_sacrifice,
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
    arkk_bomb
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
    is_buffremove = 0;
    valid_if_down = false;
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
           && special_requirement(ev,src,dst,current_player))
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

bool mechanic::special_requirement(cbtevent* &ev, ag* &src, ag* &dst, Player* &current_player)
{
    return true;
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
    fail_if_hit = false;
    ids.push_back(MECHANIC_SAB_SAPPER_BOMB); //skill id;
}

sab_time_bomb::sab_time_bomb()
{
    name="got a time bomb"; //name of mechanic
    fail_if_hit = false;
    ids.push_back(MECHANIC_SAB_TIME_BOMB); //skill id;
    valid_if_down = true;
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
    fail_if_hit = false;
    frequency_player = 12000;
}

matt_corruption::matt_corruption()
{
    name="got a corruption"; //name of mechanic
    ids.push_back(MECHANIC_MATT_CORRUPTION); //skill id;
    fail_if_hit = false;
}

matt_sacrifice::matt_sacrifice()
{
    name="is sacrificed"; //name of mechanic
    ids.push_back(MECHANIC_MATT_SACRIFICE); //skill id;
    fail_if_hit = false;
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
    valid_if_down = true;
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
    frequency_player = 1000;
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

horror_donut_inner::horror_donut_inner()
{
    name="stood in inner donut"; //name of mechanic
    ids.push_back(MECHANIC_HORROR_DONUT_INNER); //skill id;
}

horror_donut_outer::horror_donut_outer()
{
    name="stood in outer donut"; //name of mechanic
    ids.push_back(MECHANIC_HORROR_DONUT_OUTER); //skill id;
}

horror_golem_aoe::horror_golem_aoe()
{
    name="stood in torment aoe"; //name of mechanic
    ids.push_back(MECHANIC_HORROR_GOLEM_AOE); //skill id;
}

horror_pie::horror_pie()
{
    name="stood in pie slice"; //name of mechanic
    ids.push_back(MECHANIC_HORROR_PIE_4_A); //skill id;
    ids.push_back(MECHANIC_HORROR_PIE_4_B); //skill id;
}

horror_scythe::horror_scythe()
{
    name="touched a scythe"; //name of mechanic
    ids.push_back(MECHANIC_HORROR_SCYTHE); //skill id;
}

dhuum_golem::dhuum_golem()
{
    name="touched a messenger"; //name of mechanic
    ids.push_back(MECHANIC_DHUUM_GOLEM); //skill id;
}

dhuum_shackle_src::dhuum_shackle_src()
{
    name="is shackled"; //name of mechanic
    fail_if_hit = false;
    ids.push_back(MECHANIC_DHUUM_SHACKLE); //skill id;
}

dhuum_shackle_dst::dhuum_shackle_dst()
{
    name="is shackled"; //name of mechanic
    fail_if_hit = false;
    ids.push_back(MECHANIC_DHUUM_SHACKLE); //skill id;
    target_is_dst = false;
}

dhuum_affliction::dhuum_affliction()
{
    name="has affliction"; //name of mechanic
    fail_if_hit = false;
    ids.push_back(MECHANIC_DHUUM_AFFLICTION); //skill id;
    frequency_player = 13000 + ms_per_tick;
}

dhuum_crack::dhuum_crack()
{
    name="stood in a crack"; //name of mechanic
    ids.push_back(MECHANIC_DHUUM_CRACK); //skill id;
}

dhuum_mark::dhuum_mark()
{
    name="stood in a mark"; //name of mechanic
    ids.push_back(MECHANIC_DHUUM_MARK); //skill id;
}

dhuum_suck_aoe::dhuum_suck_aoe()
{
    name="touched the center during suction"; //name of mechanic
    ids.push_back(MECHANIC_DHUUM_SUCK_AOE); //skill id;
}

dhuum_teleport_aoe::dhuum_teleport_aoe()
{
    name="stood in a teleport aoe"; //name of mechanic
    ids.push_back(MECHANIC_DHUUM_TELEPORT_AOE); //skill id;
}

dhuum_snatch::dhuum_snatch()
{
    name="was snatched"; //name of mechanic
    ids.push_back(MECHANIC_DHUUM_SNATCH); //skill id;
    frequency_player = 26000;
}

nightmare_vomit::nightmare_vomit()
{
    name="vomited on someone"; //name of mechanic
    ids.push_back(MECHANIC_NIGHTMARE_VOMIT); //skill id;
    target_is_dst = false;
}

mama_wirl::mama_wirl()
{
    name="was hit by wirl"; //name of mechanic
    ids.push_back(MECHANIC_MAMA_WIRL); //skill id;
}

mama_knock::mama_knock()
{
    name="was knocked"; //name of mechanic
    ids.push_back(MECHANIC_MAMA_KNOCK); //skill id;
}

mama_leap::mama_leap()
{
    name="was leaped on"; //name of mechanic
    ids.push_back(MECHANIC_MAMA_LEAP); //skill id;
}

mama_acid::mama_acid()
{
    name="stood in acid"; //name of mechanic
    ids.push_back(MECHANIC_MAMA_ACID); //skill id;
}

mama_knight_smash::mama_knight_smash()
{
    name="was smashed by a knight"; //name of mechanic
    ids.push_back(MECHANIC_MAMA_KNIGHT_SMASH); //skill id;
}

siax_acid::siax_acid()
{
    name="stood in acid"; //name of mechanic
    ids.push_back(MECHANIC_SIAX_ACID); //skill id;
}

ensolyss_lunge::ensolyss_lunge()
{
    name="was ran over"; //name of mechanic
    ids.push_back(MECHANIC_ENSOLYSS_LUNGE); //skill id;
}

ensolyss_smash::ensolyss_smash()
{
    name="was smashed"; //name of mechanic
    ids.push_back(MECHANIC_ENSOLYSS_SMASH); //skill id;
}

arkk_pie::arkk_pie()
{
    name="stood in a pie slice"; //name of mechanic
    ids.push_back(MECHANIC_ARKK_PIE_A); //skill id;
    ids.push_back(MECHANIC_ARKK_PIE_B); //skill id;
    ids.push_back(MECHANIC_ARKK_PIE_C); //skill id;
}

arkk_fear::arkk_fear()
{
    name="was feared"; //name of mechanic
    ids.push_back(MECHANIC_ARKK_FEAR); //skill id;
}

arkk_overhead_smash::arkk_overhead_smash()
{
    name="was smashed"; //name of mechanic
    ids.push_back(MECHANIC_ARKK_OVERHEAD_SMASH); //skill id;
}

arkk_bomb::arkk_bomb()
{
    name="has a bomb"; //name of mechanic
    ids.push_back(MECHANIC_ARKK_BOMB); //skill id;
    fail_if_hit = false;
}
