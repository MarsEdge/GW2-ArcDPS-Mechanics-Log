#pragma once

#include <string>
#include <vector>

#include "arcdps_datastructures.h"
#include "player.h"
#include "mechanic_ids.h"

extern bool have_added_line_break;
extern uint64_t last_mechanic_time;
extern uint64_t line_break_frequency;
extern bool has_logged_mechanic;

const unsigned int ms_per_tick = 40;// 1000/25

struct mechanic
{
    std::string name; //name of mechanic
    std::vector<uint16_t> ids; //skill ids;
    uint64_t frequency_player; //minimum time between instances of this mechanic per player(ms)
    uint64_t frequency_global; //minimum time between instances of this mechanic globally(ms)
    uint64_t last_hit_time; //time of last instance of mechanic
    uint8_t is_buffremove;
    bool is_interupt;
    bool is_multihit = true;
    bool target_is_dst;
    bool fail_if_hit;
    bool valid_if_down; //mechanic counts if player is in down-state

    mechanic();
    bool is_valid_hit(cbtevent* &ev, ag* &src, ag* &dst);
    bool special_requirement(cbtevent* &ev, ag* &src, ag* &dst, Player* &current_player);
};

static struct vg_teleport : mechanic
{
    vg_teleport();
} vg_teleport;

static struct vg_green : mechanic
{
    vg_green();
} vg_green;

static struct gors_slam : mechanic
{
    gors_slam();
} gors_slam;

static struct gors_egg : mechanic
{
    gors_egg();
} gors_egg;

static struct sab_sapper_bomb : mechanic
{
    sab_sapper_bomb();
} sab_sapper_bomb;

static struct sab_time_bomb : mechanic
{
    sab_time_bomb();
} sab_time_bomb;

static struct sab_cannon : mechanic
{
    sab_cannon();
} sab_cannon;

static struct sab_flamewall : mechanic
{
    sab_flamewall();
} sab_flamewall;

static struct sloth_tantrum : mechanic
{
    sloth_tantrum();
} sloth_tantrum;

static struct sloth_bomb : mechanic
{
    sloth_bomb();
} sloth_bomb;

static struct sloth_bomb_aoe : mechanic
{
    sloth_bomb_aoe();
} sloth_bomb_aoe;

static struct sloth_flame : mechanic
{
    sloth_flame();
} sloth_flame;

static struct sloth_shake : mechanic
{
    sloth_shake();
} sloth_shake;

static struct matt_hadouken : mechanic
{
    matt_hadouken();
} matt_hadouken;

static struct matt_shard_reflect : mechanic
{
    matt_shard_reflect();
} matt_shard_reflect;

static struct matt_bomb : mechanic
{
    matt_bomb();
} matt_bomb;

static struct matt_corruption : mechanic
{
    matt_corruption();
} matt_corruption;

static struct matt_sacrifice : mechanic
{
    matt_sacrifice();
} matt_sacrifice;

static struct xera_half : mechanic
{
    xera_half();
} xera_half;

static struct xera_magic : mechanic
{
    xera_magic();
} xera_magic;

static struct xera_orb : mechanic
{
    xera_orb();
} xera_orb;

static struct xera_orb_aoe : mechanic
{
    xera_orb_aoe();
} xera_orb_aoe;

static struct carin_teleport : mechanic
{
    carin_teleport();
} carin_teleport;

static struct carin_shard_reflect : mechanic
{
    carin_shard_reflect();
} carin_shard_reflect;

static struct carin_green : mechanic
{
    carin_green();
} carin_green;

static struct sam_shockwave : mechanic
{
    sam_shockwave();
} sam_shockwave;

static struct sam_slap_horizontal : mechanic
{
    sam_slap_horizontal();
} sam_slap_horizontal;

static struct sam_slap_vertical : mechanic
{
    sam_slap_vertical();
} sam_slap_vertical;

static struct deimos_oil : mechanic
{
    deimos_oil();
} deimos_oil;

static struct deimos_smash : mechanic
{
    deimos_smash();
} deimos_smash;

static struct horror_donut_inner : mechanic
{
    horror_donut_inner();
} horror_donut_inner;

static struct horror_donut_outer : mechanic
{
    horror_donut_outer();
} horror_donut_outer;

static struct horror_golem_aoe : mechanic
{
    horror_golem_aoe();
} horror_golem_aoe;

static struct horror_pie : mechanic
{
    horror_pie();
} horror_pie;

static struct horror_scythe : mechanic
{
    horror_scythe();
} horror_scythe;

static struct dhuum_golem : mechanic
{
    dhuum_golem();
} dhuum_golem;

static struct dhuum_affliction : mechanic
{
    dhuum_affliction();
} dhuum_affliction;

static struct dhuum_shackle_src : mechanic
{
    dhuum_shackle_src();
} dhuum_shackle_src;

static struct dhuum_shackle_dst : mechanic
{
    dhuum_shackle_dst();
} dhuum_shackle_dst;

static struct dhuum_crack : mechanic
{
    dhuum_crack();
} dhuum_crack;

static struct dhuum_mark : mechanic
{
    dhuum_mark();
} dhuum_mark;

static struct dhuum_suck_aoe : mechanic
{
    dhuum_suck_aoe();
} dhuum_suck_aoe;

static struct dhuum_teleport_aoe : mechanic
{
    dhuum_teleport_aoe();
} dhuum_teleport_aoe;

static struct dhuum_snatch : mechanic
{
    dhuum_snatch();
} dhuum_snatch;

static struct nightmare_vomit : mechanic
{
    nightmare_vomit();
} nightmare_vomit;

static struct mama_wirl : mechanic
{
    mama_wirl();
} mama_wirl;

static struct mama_knock : mechanic
{
    mama_knock();
} mama_knock;

static struct mama_leap : mechanic
{
    mama_leap();
} mama_leap;

static struct mama_acid : mechanic
{
    mama_acid();
} mama_acid;

static struct mama_knight_smash : mechanic
{
    mama_knight_smash();
} mama_knight_smash;

static struct siax_acid : mechanic
{
    siax_acid();
} siax_acid;

static struct ensolyss_lunge : mechanic
{
    ensolyss_lunge();
} ensolyss_lunge;

static struct ensolyss_smash : mechanic
{
    ensolyss_smash();
} ensolyss_smash;

static struct arkk_pie : mechanic
{
    arkk_pie();
} arkk_pie;

static struct arkk_fear : mechanic
{
    arkk_fear();
} arkk_fear;

static struct arkk_overhead_smash : mechanic
{
    arkk_overhead_smash();
} arkk_overhead_smash;

static struct arkk_bomb : mechanic
{
    arkk_bomb();
} arkk_bomb;


extern std::vector <mechanic> mechanics;
