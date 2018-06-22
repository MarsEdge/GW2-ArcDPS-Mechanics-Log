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


std::vector<Boss> bosses =
{
    Boss().setIds({BOSS_VG_ID}).setTimer(BOSS_VG_TIMER).setHealth(BOSS_VG_HEALTH),//TODO: move health and enrage timers to header

    Boss().setIds({BOSS_GORS_ID}).setTimer(BOSS_GORS_TIMER).setHealth(BOSS_GORS_HEALTH),

    Boss().setIds({BOSS_SAB_ID}).setTimer(BOSS_SAB_TIMER).setHealth(BOSS_SAB_HEALTH),

    Boss().setIds({BOSS_SLOTH_ID}).setTimer(BOSS_SLOTH_TIMER).setHealth(BOSS_SLOTH_HEALTH),

    Boss().setIds({BOSS_TRIO_BURG_ID}).setTimer(BOSS_TRIO_TIMER),//TODO: verify enrage timer

    Boss().setIds({BOSS_MATT_ID}).setTimer(BOSS_MATT_TIMER).setHealth(BOSS_MATT_HEALTH),//TODO: verify enrage timer

    Boss().setIds({BOSS_KC_ID}).setTimer(BOSS_KC_TIMER).setHealth(BOSS_KC_HEALTH),//TODO: verify enrage timer

    Boss().setIds({BOSS_XERA_ID_A}).setTimer(BOSS_XERA_TIMER).setHealth(BOSS_XERA_HEALTH),//TODO: verify enrage timer

    Boss().setIds({BOSS_CAIRN_ID}).setTimer(BOSS_CAIRN_TIMER).setHealth(BOSS_CAIRN_HEALTH),//TODO: verify enrage timer

    Boss().setIds({BOSS_MO_ID}).setTimer(BOSS_MO_TIMER).setHealth(BOSS_MO_HEALTH),//TODO: verify enrage timer

    Boss().setIds({BOSS_SAM_ID}).setTimer(BOSS_SAM_TIMER).setHealth(BOSS_SAM_HEALTH),//TODO: verify enrage timer

    Boss().setIds({BOSS_DEIMOS_ID}).setTimer(BOSS_DEIMOS_TIMER).setHealth(BOSS_DEIMOS_HEALTH),//TODO: verify enrage timer

    Boss().setIds({BOSS_SH_ID}).setTimer(BOSS_SH_TIMER).setHealth(BOSS_SH_HEALTH),

	Boss().setIds({BOSS_SOUL_EATER_ID}).setHealth(BOSS_SOUL_EATER_HEALTH),

	Boss().setIds({BOSS_ICE_KING_ID}).setTimer(BOSS_ICE_KING_TIMER).setHealth(BOSS_ICE_KING_HEALTH),

	Boss().setIds({BOSS_EYE_NORTH_ID,BOSS_EYE_SOUTH_ID}).setHealth(BOSS_EYE_HEALTH),

    Boss().setIds({BOSS_DHUUM_ID}).setTimer(BOSS_DHUUM_TIMER).setHealth(BOSS_DHUUM_HEALTH),


    Boss().setIds({BOSS_MAMA_ID,BOSS_MAMA_ID_NORMAL,BOSS_MAMA_ID_TRASH}).setHealth(BOSS_MAMA_HEALTH),

    Boss().setIds({BOSS_SIAX_ID}).setHealth(BOSS_SIAX_HEALTH),

    Boss().setIds({BOSS_ENSOLYSS_ID}).setHealth(BOSS_ENSOLYSS_HEALTH),


    Boss().setIds({BOSS_SKORVALD_ID}).setHealth(BOSS_SKORVALD_HEALTH),

    Boss().setIds({BOSS_ARTSARIIV_ID}).setHealth(BOSS_ARTSARIIV_HEALTH),

    Boss().setIds({BOSS_ARKK_ID}).setHealth(BOSS_ARKK_HEALTH),
};
