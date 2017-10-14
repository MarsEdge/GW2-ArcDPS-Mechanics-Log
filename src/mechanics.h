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

struct mechanic
{
    std::string name; //name of mechanic
    std::vector<uint16_t> ids; //skill ids;
    uint64_t frequency; //minimum time between instances of this mechanic(ms)
    bool is_interupt;
    bool is_multihit = true;
    bool target_is_dst;
    bool fail_if_hit;

    mechanic();
    bool is_valid_hit(cbtevent* &ev, ag* &src, ag* &dst);
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

static struct sam_shockwave : mechanic
{
    sam_shockwave();
} sam_shockwave;

static struct sam_slap : mechanic
{
    sam_slap();
} sam_slap;

static struct deimos_oil : mechanic
{
    uint64_t last_touched_time;
    uint16_t last_oil_id;

    deimos_oil();
    bool is_valid_hit(cbtevent* &ev, ag* &src, ag* &dst);
} deimos_oil;

static struct deimos_smash : mechanic
{
    deimos_smash();
} deimos_smash;

extern std::vector <mechanic> mechanics;
