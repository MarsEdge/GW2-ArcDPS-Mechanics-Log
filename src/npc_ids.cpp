#include "npc_ids.h"

boss::boss()
{
    ids = std::vector<uint32_t>();
    name = "";
    timer = 0;
    health = 0;
}


std::vector<boss> bosses =
{
    boss().set_ids({ID_vale_guardian}).set_timer(8*60*1000).set_health(22021440),//TODO: move health and enrage timers to header

    boss().set_ids({ID_gorseval}).set_timer(7*60*1000).set_health(21628200),

    boss().set_ids({ID_sabetha}).set_timer(9*60*1000).set_health(34015256),

    boss().set_ids({ID_slothasor}).set_timer(7*60*1000).set_health(18973828),

    boss().set_ids({ID_trio_burg}).set_timer(9*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_matthias}).set_timer(10*60*1000).set_health(25953840),//TODO: verify enrage timer

    boss().set_ids({ID_keep_construct}).set_timer(10*60*1000).set_health(55053600),//TODO: verify enrage timer

    boss().set_ids({ID_xera_part1}).set_timer(11*60*1000).set_health(22611300),//TODO: verify enrage timer

    boss().set_ids({ID_cairn}).set_timer(8*60*1000).set_health(19999998),//TODO: verify enrage timer

    boss().set_ids({ID_overseer}).set_timer(6*60*1000).set_health(22021440),//TODO: verify enrage timer

    boss().set_ids({ID_samarog}).set_timer(11*60*1000).set_health(29493000),//TODO: verify enrage timer

    boss().set_ids({ID_deimos}).set_timer(12*60*1000).set_health(50049000),//TODO: verify enrage timer

    boss().set_ids({ID_soulless_horror}).set_timer(8*60*1000).set_health(35391600),

    boss().set_ids({ID_dhuum}).set_timer(10*60*1000).set_health(32000000)
};
