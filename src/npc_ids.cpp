#include "npc_ids.h"

boss::boss()
{
    ids = std::vector<uint32_t>();
    name = "";
    timer = 0;
}


std::vector<boss> bosses =
{
    boss().set_ids({ID_vale_guardian}).set_timer(8*60*1000),

    boss().set_ids({ID_gorseval}).set_timer(7*60*1000),

    boss().set_ids({ID_sabetha}).set_timer(9*60*1000),

    boss().set_ids({ID_slothasor}).set_timer(7*60*1000),

    boss().set_ids({ID_trio_burg}).set_timer(9*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_matthias}).set_timer(10*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_keep_construct}).set_timer(10*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_xera_part1}).set_timer(11*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_cairn}).set_timer(8*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_overseer}).set_timer(6*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_samarog}).set_timer(11*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_deimos}).set_timer(12*60*1000),//TODO: verify enrage timer

    boss().set_ids({ID_soulless_horror}).set_timer(8*60*1000),

    boss().set_ids({ID_dhuum}).set_timer(10*60*1000)
};
