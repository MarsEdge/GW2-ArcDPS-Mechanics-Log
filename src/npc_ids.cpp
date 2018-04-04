#include "npc_ids.h"

boss::boss()
{
    ids = std::vector<uint32_t>();
    name = "";
    timer = 0;
    health = 0;
	pulls = 0;
}

bool boss::has_id(uint32_t new_id)
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


std::vector<boss> bosses =
{
    boss().set_ids({BOSS_VG_ID}).set_timer(BOSS_VG_TIMER).set_health(BOSS_VG_HEALTH),//TODO: move health and enrage timers to header

    boss().set_ids({BOSS_GORS_ID}).set_timer(BOSS_GORS_TIMER).set_health(BOSS_GORS_HEALTH),

    boss().set_ids({BOSS_SAB_ID}).set_timer(BOSS_SAB_TIMER).set_health(BOSS_SAB_HEALTH),

    boss().set_ids({BOSS_SLOTH_ID}).set_timer(BOSS_SLOTH_TIMER).set_health(BOSS_SLOTH_HEALTH),

    boss().set_ids({BOSS_TRIO_BURG_ID}).set_timer(BOSS_TRIO_TIMER),//TODO: verify enrage timer

    boss().set_ids({BOSS_MATT_ID}).set_timer(BOSS_MATT_TIMER).set_health(BOSS_MATT_HEALTH),//TODO: verify enrage timer

    boss().set_ids({BOSS_KC_ID}).set_timer(BOSS_KC_TIMER).set_health(BOSS_KC_HEALTH),//TODO: verify enrage timer

    boss().set_ids({BOSS_XERA_ID_A}).set_timer(BOSS_XERA_TIMER).set_health(BOSS_XERA_HEALTH),//TODO: verify enrage timer

    boss().set_ids({BOSS_CAIRN_ID}).set_timer(BOSS_CAIRN_TIMER).set_health(BOSS_CAIRN_HEALTH),//TODO: verify enrage timer

    boss().set_ids({BOSS_MO_ID}).set_timer(BOSS_MO_TIMER).set_health(BOSS_MO_HEALTH),//TODO: verify enrage timer

    boss().set_ids({BOSS_SAM_ID}).set_timer(BOSS_SAM_TIMER).set_health(BOSS_SAM_HEALTH),//TODO: verify enrage timer

    boss().set_ids({BOSS_DEIMOS_ID}).set_timer(BOSS_DEIMOS_TIMER).set_health(BOSS_DEIMOS_HEALTH),//TODO: verify enrage timer

    boss().set_ids({BOSS_SH_ID}).set_timer(BOSS_SH_TIMER).set_health(BOSS_SH_HEALTH),

	boss().set_ids({BOSS_SOUL_EATER_ID}).set_health(BOSS_SOUL_EATER_HEALTH),

	boss().set_ids({BOSS_ICE_KING_ID}).set_timer(BOSS_ICE_KING_TIMER).set_health(BOSS_ICE_KING_HEALTH),

	boss().set_ids({BOSS_EYE_NORTH_ID,BOSS_EYE_SOUTH_ID}).set_health(BOSS_EYE_HEALTH),

    boss().set_ids({BOSS_DHUUM_ID}).set_timer(BOSS_DHUUM_TIMER).set_health(BOSS_DHUUM_HEALTH),


    boss().set_ids({BOSS_MAMA_ID,BOSS_MAMA_ID_NORMAL,BOSS_MAMA_ID_TRASH}).set_health(BOSS_MAMA_HEALTH),

    boss().set_ids({BOSS_SIAX_ID}).set_health(BOSS_SIAX_HEALTH),

    boss().set_ids({BOSS_ENSOLYSS_ID}).set_health(BOSS_ENSOLYSS_HEALTH),


    boss().set_ids({BOSS_SKORVALD_ID}).set_health(BOSS_SKORVALD_HEALTH),

    boss().set_ids({BOSS_ARTSARIIV_ID}).set_health(BOSS_ARTSARIIV_HEALTH),

    boss().set_ids({BOSS_ARKK_ID}).set_health(BOSS_ARKK_HEALTH),
};
