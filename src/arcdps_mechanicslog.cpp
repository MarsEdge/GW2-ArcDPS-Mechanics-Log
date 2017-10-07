#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include <vector>
#include <string>
#include "mechanic_ids.h"
#include "player.cpp"

#define MAX_PLAYER_COUNT 10

/* arcdps export table */
typedef struct arcdps_exports {
	uintptr_t size; /* arcdps internal use, ignore */
	uintptr_t sig; /* pick a number between 0 and uint64_t max that isn't used by other modules */
	char* out_name; /* name string */
	char* out_build; /* build string */
	void* wnd; /* wndproc callback, fn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) */
	void* combat; /* combat event callback, fn(cbtevent* ev, ag* src, ag* dst, char* skillname) */
	void* imgui; /* id3dd9::present callback, before imgui::render, fn() */
	void* options; /* id3dd9::present callback, appending to the end of options window in arcdps, fn() */
} arcdps_exports;

/* combat event */
typedef struct cbtevent {
	uint64_t time; /* timegettime() at time of event */
	uint64_t src_agent; /* unique identifier */
	uint64_t dst_agent; /* unique identifier */
	int32_t value; /* event-specific */
	int32_t buff_dmg; /* estimated buff damage. zero on application event */
	uint16_t overstack_value; /* estimated overwritten stack duration for buff application */
	uint16_t skillid; /* skill id */
	uint16_t src_instid; /* agent map instance id */
	uint16_t dst_instid; /* agent map instance id */
	uint16_t src_master_instid; /* master source agent map instance id if source is a minion/pet */
	uint8_t iss_offset; /* internal tracking. garbage */
	uint8_t iss_offset_target; /* internal tracking. garbage */
	uint8_t iss_bd_offset; /* internal tracking. garbage */
	uint8_t iss_bd_offset_target; /* internal tracking. garbage */
	uint8_t iss_alt_offset; /* internal tracking. garbage */
	uint8_t iss_alt_offset_target; /* internal tracking. garbage */
	uint8_t skar; /* internal tracking. garbage */
	uint8_t skar_alt; /* internal tracking. garbage */
	uint8_t skar_use_alt; /* internal tracking. garbage */
	uint8_t iff; /* from iff enum */
	uint8_t buff; /* buff application, removal, or damage event */
	uint8_t result; /* from cbtresult enum */
	uint8_t is_activation; /* from cbtactivation enum */
	uint8_t is_buffremove; /* buff removed. src=relevant, dst=caused it (for strips/cleanses). from cbtr enum  */
	uint8_t is_ninety; /* source agent health was over 90% */
	uint8_t is_fifty; /* target agent health was under 50% */
	uint8_t is_moving; /* source agent was moving */
	uint8_t is_statechange; /* from cbtstatechange enum */
	uint8_t is_flanking; /* target agent was not facing source */
	uint8_t is_shields; /* all or partial damage was vs barrier/shield */
	uint8_t pad63; /* internal tracking. garbage */
	uint8_t pad64; /* internal tracking. garbage */
} cbtevent;

/* agent short */
typedef struct ag {
	char* name; /* agent name. may be null. valid only at time of event. utf8 */
	uintptr_t id; /* agent unique identifier */
	uint32_t prof; /* profession at time of event. refer to evtc notes for identification */
	uint32_t elite; /* elite spec at time of event. refer to evtc notes for identification */
	uint32_t self; /* 1 if self, 0 if not */
} ag;

/* proto/globals */
uint32_t cbtcount = 0;
arcdps_exports arc_exports;
char* arcvers;
void dll_init(HANDLE hModule);
void dll_exit();
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversionstr, void* imguicontext);
extern "C" __declspec(dllexport) void* get_release_addr();
arcdps_exports* mod_init();
uintptr_t mod_release();
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname);
uintptr_t mod_imgui();
uintptr_t mod_options();

Player players[MAX_PLAYER_COUNT] = {Player()};
Player* get_player(uint16_t new_id);
void reset_all_player_stats();
bool is_player(ag* new_player);
const unsigned int ms_per_tick = 40;// 1000/25
uint64_t start_time = 0;
bool have_added_line_break = true;
uint64_t last_mechanic_time = 0;
uint64_t line_break_frequency = 5000;
bool has_logged_mechanic = false;

struct mechanic
{
    std::string name = ""; //name of mechanic
    std::vector<uint16_t> ids; //skill ids;
    uint64_t frequency=2000; //minimum time between instances of this mechanic(ms)
    bool is_interupt=false;
    bool target_is_dst = true;
    bool fail_if_hit = true;

    bool is_valid_hit(cbtevent* &ev, ag* &src, ag* &dst)
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
            if(target_is_dst && is_player(dst))
            {
                current_player=get_player(ev->dst_instid);
            }
            else if(!target_is_dst && is_player(src))
            {
                current_player=get_player(ev->src_instid);
            }

            if(current_player
               && ev->time > (current_player->last_hit_time+this->frequency)
               && (!is_interupt || current_player->last_stab_time < ev->time))
            {
                current_player->last_hit_time=ev->time;
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
};

struct vg_teleport : mechanic
{
    vg_teleport()
    {
        name="was teleported"; //name of mechanic
        ids.push_back(MECHANIC_VG_TELEPORT_RAINBOW);
        ids.push_back(MECHANIC_VG_TELEPORT_GREEN);
    }
} vg_teleport;

struct vg_green : mechanic
{
    vg_green()
    {
        name="stood in the green circle"; //name of mechanic
        ids.push_back(MECHANIC_VG_GREEN_A);
        ids.push_back(MECHANIC_VG_GREEN_B);
        ids.push_back(MECHANIC_VG_GREEN_C);
        ids.push_back(MECHANIC_VG_GREEN_D);
        fail_if_hit = false;
    }
} vg_green;

struct gors_slam : mechanic
{
    gors_slam()
    {
        name="was slammed"; //name of mechanic
        ids.push_back(MECHANIC_GORS_SLAM); //skill id;
        is_interupt=true;
    }
} gors_slam;

struct gors_egg : mechanic
{
    gors_egg()
    {
        name="was egged"; //name of mechanic
        ids.push_back(MECHANIC_GORS_EGG); //skill id;
    }

} gors_egg;

struct sloth_tantrum : mechanic
{
    sloth_tantrum()
    {
        name="was hit with tantrum"; //name of mechanic
        ids.push_back(MECHANIC_SLOTH_TANTRUM); //skill id;
    }

} sloth_tantrum;

struct sloth_bomb : mechanic
{
    sloth_bomb()
    {
        name="got a bomb"; //name of mechanic
        ids.push_back(MECHANIC_SLOTH_BOMB); //skill id;
        fail_if_hit = false;
        frequency = 6000;
    }

} sloth_bomb;

struct sloth_bomb_aoe : mechanic
{
    sloth_bomb_aoe()
    {
        name="stood in a bomb aoe"; //name of mechanic
        ids.push_back(MECHANIC_SLOTH_BOMB_AOE); //skill id;
    }

} sloth_bomb_aoe;

struct sloth_flame : mechanic
{
    sloth_flame()
    {
        name="was hit by flame breath"; //name of mechanic
        ids.push_back(MECHANIC_SLOTH_FLAME_BREATH); //skill id;
    }

} sloth_flame;

struct sloth_shake : mechanic
{
    sloth_shake()
    {
        name="was hit by shake"; //name of mechanic
        ids.push_back(MECHANIC_SLOTH_SHAKE); //skill id;
    }

} sloth_shake;

struct matt_hadouken : mechanic
{
    matt_hadouken()
    {
        name="was hadoukened"; //name of mechanic
        ids.push_back(MECHANIC_MATT_HADOUKEN_HUMAN); //skill id;
        ids.push_back(MECHANIC_MATT_HADOUKEN_ABOM); //skill id;
    }

} matt_hadouken;

struct matt_shard_reflect : mechanic
{
    matt_shard_reflect()
    {
        name="reflected shards"; //name of mechanic
        ids.push_back(MECHANIC_MATT_SHARD_HUMAN); //skill id;
        ids.push_back(MECHANIC_MATT_SHARD_ABOM); //skill id;
        target_is_dst = false;
    }

} matt_shard_reflect;

struct matt_bomb : mechanic
{
    matt_bomb()
    {
        name="got a bomb"; //name of mechanic
        ids.push_back(MECHANIC_MATT_BOMB); //skill id;
        frequency = 12000;
    }

} matt_bomb;

struct xera_half : mechanic
{
    xera_half()
    {
        name="stood in the red half"; //name of mechanic
        ids.push_back(MECHANIC_XERA_HALF); //skill id;
    }

} xera_half;

struct xera_magic : mechanic
{
    xera_magic()
    {
        name="has magic"; //name of mechanic
        ids.push_back(MECHANIC_XERA_MAGIC); //skill id;
        frequency=5000; //the bubbles don't happen very often
        fail_if_hit = false;
    }

} xera_magic;

struct xera_orb : mechanic
{
    xera_orb()
    {
        name="touched an orb"; //name of mechanic
        ids.push_back(MECHANIC_XERA_ORB); //skill id;
    }

} xera_orb;

struct xera_orb_aoe : mechanic
{
    xera_orb_aoe()
    {
        name="stood in an orb aoe"; //name of mechanic
        ids.push_back(MECHANIC_XERA_ORB_AOE); //skill id;
    }

} xera_orb_aoe;

struct carin_teleport : mechanic
{
    carin_teleport()
    {
        name="was teleported"; //name of mechanic
        ids.push_back(MECHANIC_CARIN_TELEPORT); //skill id;
    }

} carin_teleport;

struct carin_shard_reflect : mechanic
{
    carin_shard_reflect()
    {
        name="reflected shards"; //name of mechanic
        ids.push_back(MECHANIC_CARIN_SHARD); //skill id;
        target_is_dst = false;
    }

} carin_shard_reflect;

struct sam_shockwave : mechanic
{
    sam_shockwave()
    {
        name="was hit by shockwave"; //name of mechanic
        ids.push_back(MECHANIC_SAM_SHOCKWAVE); //skill id;
        is_interupt=true;
    }

} sam_shockwave;

struct sam_slap : mechanic
{
    sam_slap()
    {
        name="was slapped"; //name of mechanic
        ids.push_back(MECHANIC_SAM_SLAP); //skill id;
        is_interupt=true;
    }

} sam_slap;

struct deimos_oil : mechanic
{
    uint64_t last_touched_time = 0;
    uint16_t last_oil_id = 0;

    deimos_oil()
    {
        name="touched an oil"; //name of mechanic
        ids.push_back(MECHANIC_DEIMOS_OIL); //skill id;
    }
    bool is_valid_hit(cbtevent* &ev, ag* &src, ag* &dst)
    {
        if(mechanic::is_valid_hit(ev, src, dst)) //TODO: this will count players failing mechanics
        {                                        // more than is accurate for internal tracking
            if(last_touched_time > (ev->time+frequency)
               && last_oil_id == ev->src_instid)
            {
                last_touched_time = ev->time;
                return false;
            }
            else if(last_oil_id != ev->src_instid)
            {
                last_oil_id = ev->src_instid;
            }
            return true;
        }
        return false;
    }

} deimos_oil;

struct deimos_smash : mechanic
{
    deimos_smash()
    {
        name="was hit by smash"; //name of mechanic
        ids.push_back(MECHANIC_DEIMOS_SMASH); //skill id;
        ids.push_back(MECHANIC_DEIMOS_SMASH_INITIAL);
        is_interupt=true;
    }

} deimos_smash;

std::vector <mechanic> mechanics =
{
    vg_teleport,
//    vg_green,
    gors_slam,
    gors_egg,
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
    sam_shockwave,
    sam_slap,
    deimos_oil,
    deimos_smash
};

Player* get_player(uint16_t new_id)
{
    for(unsigned int index=0;index<MAX_PLAYER_COUNT;index++)
    {
        if(players[index].id==0)
        {
            players[index] = Player(new_id);
            return &players[index];
        }
        else if(players[index].id == new_id)
        {
            return &players[index];
        }
    }
    return nullptr;
}

void reset_all_player_stats()
{
    for(unsigned int index=0;index<MAX_PLAYER_COUNT;index++)
    {
        players[index].reset_all();
    }
}

inline int get_elapsed_time(uint64_t &current_time)
{
    return (current_time-start_time)/1000;
}

inline bool is_player(ag* new_player)
{
    return new_player->prof < 10;
}

/* dll main -- winapi */
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ulReasonForCall, LPVOID lpReserved) {
	switch (ulReasonForCall) {
		case DLL_PROCESS_ATTACH: dll_init(hModule); break;
		case DLL_PROCESS_DETACH: dll_exit(); break;

		case DLL_THREAD_ATTACH:  break;
		case DLL_THREAD_DETACH:  break;
	}
	return 1;
}

/* dll attach -- from winapi */
void dll_init(HANDLE hModule) {
	return;
}

/* dll detach -- from winapi */
void dll_exit() {
	return;
}

/* export -- arcdps looks for this exported function and calls the address it returns */
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversionstr, void* imguicontext) {
	arcvers = arcversionstr;
	//ImGui::SetCurrentContext((ImGuiContext*)imguicontext);
	return mod_init;
}

/* export -- arcdps looks for this exported function and calls the address it returns */
extern "C" __declspec(dllexport) void* get_release_addr() {
	arcvers = 0;
	return mod_release;
}

/* initialize mod -- return table that arcdps will use for callbacks */
arcdps_exports* mod_init() {

	/* demo */
	AllocConsole();

	/* big buffer */
	char buff[4096];
	char* p = &buff[0];
	p += _snprintf(p, 400, "==== mechanics log ====\n");

	/* print */
	DWORD written = 0;
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(hnd, &buff[0], p - &buff[0], &written, 0);

	/* for arcdps */
	arc_exports.size = sizeof(arcdps_exports);
	arc_exports.out_name = "mechanics log";
	arc_exports.out_build = "0.1";
	arc_exports.sig = 0x81004122;//from random.org
	arc_exports.wnd = mod_wnd;
	arc_exports.combat = mod_combat;
	arc_exports.imgui = mod_imgui;
	arc_exports.options = mod_options;
	return &arc_exports;
}

/* release mod -- return ignored */
uintptr_t mod_release() {
	FreeConsole();
	return 0;
}

/* window callback -- return is assigned to umsg (return zero to not be processed by arcdps or game) */
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
#if 0
	/* big buffer */
	char buff[4096];
	char* p = &buff[0];

	/* common */
	p += _snprintf(p, 400, "==== wndproc %llx ====\n", hWnd);
	p += _snprintf(p, 400, "umsg %u, wparam %lld, lparam %lld\n", uMsg, wParam, lParam);

	/* print */
	DWORD written = 0;
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(hnd, &buff[0], p - &buff[0], &written, 0);
#endif
	return uMsg;
}

/* combat callback -- may be called asynchronously. return ignored */
/* one participant will be party/squad, or minion of. no spawn statechange events. despawn statechange only on marked boss npcs */
uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname) {

	/* big buffer */
	char buff[4096];
	char* p = &buff[0];
	Player* current_player = nullptr;

	/* ev is null. dst will only be valid on tracking add. skillname will also be null */
	if (!ev)
    {

	}

	/* combat event. skillname may be null. non-null skillname will remain static until module is unloaded. refer to evtc notes for complete detail */
	else
    {

		/* common */

		/* statechange */
		if (ev->is_statechange)
        {
            if(ev->is_statechange==1
                && src->self)
            {
                    start_time = ev->time;
                    reset_all_player_stats();
                    if(has_logged_mechanic)
                    {
                        has_logged_mechanic = false;
                        p +=  _snprintf(p, 400, "-----------\n");
                    }
            }
		}

		/* activation */
		else if (ev->is_activation)
        {

		}

		/* buff remove */
		else if (ev->is_buffremove)
        {
            if(is_player(dst))
            {
                if (ev->skillid==1122)//if it's stability
                {
                    current_player=get_player(ev->dst_instid);
                    if(current_player)
                    {
                        current_player->last_stab_time = ev->time+ms_per_tick;//cut the ending time of stab early
                    }
                }
            }
		}

		/* buff */
		else if (ev->buff)
        {
            if(is_player(dst))
            {
                if (ev->skillid==1122)//if it's stability
                {
                    current_player=get_player(ev->dst_instid);
                    if(current_player
                       && current_player->last_stab_time < (ev->time+ev->value))//if the new stab will last longer than any possible old stab
                    {
                        current_player->last_stab_time = ev->time+ev->value+ms_per_tick;//add prediction of when new stab will end
                    }
                }
            }
		}

        if(ev->dst_agent)
        {
            //if attack hits (not block/evaded/invuln/miss)
            //and it's a player, not a summon
            if(ev->result==0 || ev->result==1 || ev->result==2 || ev->result==5 || ev->result==8)
            {

                for(uint16_t index=0;index<mechanics.size();index++)
                {
                    if(mechanics[index].is_valid_hit(ev, src, dst))
                    {
                        if(mechanics[index].target_is_dst)
                        {
                            p +=  _snprintf(p, 400, "%d:%d - %s %s\n",get_elapsed_time(ev->time)/60, get_elapsed_time(ev->time)%60, dst->name,mechanics[index].name.c_str());
                        }
                        else
                        {
                            p +=  _snprintf(p, 400, "%d:%d - %s %s\n",get_elapsed_time(ev->time)/60, get_elapsed_time(ev->time)%60, src->name,mechanics[index].name.c_str());
                        }
                    }
                }
            }
        }

		/* common */
		cbtcount += 1;

		if(!have_added_line_break
        && ev->time > (last_mechanic_time + line_break_frequency))
        {
            have_added_line_break = true;
            p +=  _snprintf(p, 400, "\n");
        }
	}

	/* print */
	DWORD written = 0;
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(hnd, &buff[0], p - &buff[0], &written, 0);
	return 0;
}

uintptr_t mod_imgui()
{
    return 0;
}

uintptr_t mod_options()
{
    return 0;
}
