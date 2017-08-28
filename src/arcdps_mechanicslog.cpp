/*
* arcdps combat api example
*/

#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include <vector>
#include "mechanic_ids.h"
#include "player.cpp"

/* arcdps export table */
typedef struct arcdps_exports {
	uintptr_t ext_size; /* arcdps internal use, ignore */
	uintptr_t ext_sig; /* pick a number between 0 and uint64_t max that isn't used by other modules */
	void* ext_wnd; /* wndproc callback, fn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) */
	void* ext_combat; /* combat event callback, fn(cbtevent* ev, ag* src, ag* dst, char* skillname) */
	void* ext_imgui; /* id3dd9::present callback, before imgui::render, fn() */
	void* ext_options; /* id3dd9::present callback, appending to the end of options window in arcdps, fn() */
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
	uint8_t result_local; /* internal tracking. garbage */
	uint8_t ident_local; /* internal tracking. garbage */
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
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversionstr);
extern "C" __declspec(dllexport) void* get_release_addr();
arcdps_exports* mod_init();
uintptr_t mod_release();
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname);
uintptr_t mod_imgui();
uintptr_t mod_options();

std::vector<Player> players;
Player* get_player(uint16_t new_id);
void reset_all_player_stats();
Player* current_player = nullptr;

struct mechanic
{
    char* name; //name of mechanic
    uint16_t id; //skill id;
    uint64_t frequency=2000; //minimum time between instances of this mechanic(ms)
    bool is_interupt=false;

    bool is_valid_hit(uint64_t &time, uint16_t &skillid, uint16_t &target, uint8_t &result)
    {
        if(skillid==this->id)//correct skill id
                //&& (!is_interupt || result==5)
       {
            current_player=get_player(target);

            if(current_player != nullptr
               && time > (current_player->last_hit_time+this->frequency)
               && (!is_interupt || current_player->last_stab_time < time))
            {
                current_player->last_hit_time=time;
                current_player->mechanics_failed++;
                current_player->last_machanic=skillid;
                return true;
            }
       }
       return false;
    }
};

struct vg_teleport : mechanic
{
    uint16_t id_A=MECHANIC_VG_RAINBOW_TELEPORT; //skill id;
    uint16_t id_B=MECHANIC_VG_GREEN_TELEPORT; //skill id;

    vg_teleport()
    {
        name="teleport"; //name of mechanic
        id = id_A;
    }

    //rainbow vg and green vg have a different skill id
    //so gotta overload this to check both ids
    bool is_valid_hit(uint64_t &time, uint16_t &skillid, uint16_t &target, uint8_t &result)
    {
        if( skillid == id_B)
        {
            return mechanic::is_valid_hit(time, id_A, target, result);
        }
        else
        {
            return mechanic::is_valid_hit(time, skillid, target, result);
        }
    }
} vg_teleport;

struct gors_slam : mechanic
{
    gors_slam()
    {
        name="slam"; //name of mechanic
        id=MECHANIC_GORS_SLAM; //skill id;
        is_interupt=true;
    }
} gors_slam;

struct gors_egg : mechanic
{
    gors_egg()
    {
        name="egg"; //name of mechanic
        id=MECHANIC_GORS_EGG; //skill id;
    }

} gors_egg;

struct matt_hadouken : vg_teleport
{
    matt_hadouken()
    {
        name="hadouken"; //name of mechanic
        id_A=MECHANIC_MATT_HADOUKEN_HUMAN; //skill id;
        id_B=MECHANIC_MATT_HADOUKEN_ABOM; //skill id;
    }

} matt_hadouken;

struct xera_magic : mechanic
{
    xera_magic()
    {
        name="magic"; //name of mechanic
        id=MECHANIC_XERA_MAGIC; //skill id;
        frequency=5000; //the bubbles don't happen very often
    }

} xera_magic;

struct carin_teleport : mechanic
{
    carin_teleport()
    {
        name="teleport"; //name of mechanic
        id=MECHANIC_CARIN_TELEPORT; //skill id;
    }

} carin_teleport;

struct sam_shockwave : mechanic
{
    sam_shockwave()
    {
        name="shockwave"; //name of mechanic
        id=MECHANIC_SAM_SHOCKWAVE; //skill id;
        is_interupt=true;
    }

} sam_shockwave;

struct sam_slap : mechanic
{
    sam_slap()
    {
        name="slap"; //name of mechanic
        id=MECHANIC_SAM_SLAP; //skill id;
        is_interupt=true;
    }

} sam_slap;

struct deimos_oil : mechanic
{
    uint64_t last_time=0;

    deimos_oil()
    {
        name="oil"; //name of mechanic
        id=MECHANIC_DEIMOS_OIL; //skill id;
    }

    bool is_valid_hit(uint64_t &time, uint16_t &skillid, uint16_t &target, uint8_t &result)
    {
        if(skillid==this->id)
        {
            this->last_time=time;
        }
        return mechanic::is_valid_hit(time, skillid, target,result);
    }

} deimos_oil;

struct deimos_smash : mechanic
{
    deimos_smash()
    {
        name="smash"; //name of mechanic
        id=MECHANIC_DEIMOS_SMASH; //skill id;
        is_interupt=true;
    }

} deimos_smash;

Player* get_player(uint16_t new_id)
{
    for(int index=0;index<players.size();index++)
    {
        if(players[index].id == new_id)
        {
            return &players[index];
        }
    }
    players.push_back(Player(new_id));
    return &players.back();
}

void reset_all_player_stats()
{
    players.clear();
}

uint64_t start_time = 0;

inline int get_elapsed_time(uint64_t &current_time){
    return ((int)(current_time-start_time))/1000;
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
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversionstr) {
	arcvers = arcversionstr;
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
	p += _snprintf(p, 400, "==== mod_init ====\n");

	/* print */
	DWORD written = 0;
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(hnd, &buff[0], p - &buff[0], &written, 0);

	/* for arcdps */
	arc_exports.ext_size = sizeof(arcdps_exports);
	arc_exports.ext_sig = 0x81004122;//from random.org
	arc_exports.ext_wnd = mod_wnd;
	arc_exports.ext_combat = mod_combat;
	arc_exports.ext_imgui = mod_imgui;
	arc_exports.ext_options = mod_options;
	return &arc_exports;
}

/* release mod -- return ignored */
uintptr_t mod_release() {
	FreeConsole();
	return 0;
}

/* window callback -- return is assigned to umsg (return zero to not be processed by arcdps or game) */
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	/* big buffer */
	char buff[4096];
	char* p = &buff[0];

	/* common */
	p += _snprintf(p, 400, "==== wndproc %llx ====\n", hWnd);
	p += _snprintf(p, 400, "umsg %u, wparam %lld, lparam %lld\n", uMsg, wParam, lParam);

	/* print */
	DWORD written = 0;
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	//WriteConsoleA(hnd, &buff[0], p - &buff[0], &written, 0);
	return uMsg;
}

/* combat callback -- may be called asynchronously. return ignored */
/* one participant will be party/squad, or minion of. no spawn statechange events. despawn statechange only on marked boss npcs */
uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname) {

	/* big buffer */
	char buff[4096];
	char* p = &buff[0];

	/* ev is null. dst will only be valid on tracking add. skillname will also be null */
	if (!ev) {

	}

	/* combat event. skillname may be null. non-null skillname will remain static until module is unloaded. refer to evtc notes for complete detail */
	else {

		/* common */

		/* statechange */
		if (ev->is_statechange)
        {
            if(ev->is_statechange==1
                && src->self)
            {
                    start_time = ev->time;
                    reset_all_player_stats();
            }
		}

		/* activation */
		else if (ev->is_activation) {
		}

		/* buff remove */
		else if (ev->is_buffremove) {
            if (ev->skillid==1122)//if it's stability
            {
                current_player=get_player(ev->dst_instid);
                current_player->last_stab_time = ev->time;//cut the ending time of stab early
            }
		}

		/* buff */
		else if (ev->buff) {
            if (ev->skillid==1122)//if it's stability
            {
                current_player=get_player(ev->dst_instid);
                if(current_player->last_stab_time < (ev->time+ev->value))//if the new stab will last longer than any possible old stab
                {
                    current_player->last_stab_time = ev->time+ev->value;//add prediction of when new stab will end
                }
            }
		}

        if(ev->dst_agent) {
            //if attack hits (not block/evaded/invuln/miss)
            //and it's a player, not a summon
            if((ev->result==0 || ev->result==1 || ev->result==2 || ev->result==5 || ev->result==8)
               && dst->prof <10){

                //vg teleport
                if(vg_teleport.is_valid_hit(ev->time,ev->skillid,ev->dst_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s was teleported\n",get_elapsed_time(ev->time), dst->name);
                }

                //gors slam
                if(gors_slam.is_valid_hit(ev->time,ev->skillid,ev->dst_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s was slammed\n",get_elapsed_time(ev->time), dst->name);
                }

                //gors egg
                if(gors_egg.is_valid_hit(ev->time,ev->skillid,ev->dst_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s was egged\n",get_elapsed_time(ev->time), dst->name);
                }

                //matti hadouken
                if(matt_hadouken.is_valid_hit(ev->time,ev->skillid,ev->dst_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s was hadoukened\n",get_elapsed_time(ev->time), dst->name);
                }

                //xera magic
                if(xera_magic.is_valid_hit(ev->time,ev->skillid,ev->dst_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s has magic\n",get_elapsed_time(ev->time), dst->name);
                }

                //carin teleport
                if(carin_teleport.is_valid_hit(ev->time,ev->skillid,ev->dst_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s was teleported\n",get_elapsed_time(ev->time), dst->name);
                }

                //sam shockwave
                if(sam_shockwave.is_valid_hit(ev->time,ev->skillid,ev->dst_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s was hit by shockwave\n",get_elapsed_time(ev->time), dst->name);
                }

                //sam slap
                if(sam_slap.is_valid_hit(ev->time,ev->skillid,ev->dst_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s was slapped\n",get_elapsed_time(ev->time), dst->name);
                }

                //deimos oil
                if(deimos_oil.is_valid_hit(ev->time,ev->skillid,ev->src_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s touched an oil\n",get_elapsed_time(ev->time), dst->name);
                }

                //deimos smash
                if(deimos_smash.is_valid_hit(ev->time,ev->skillid,ev->src_instid,ev->result)) {
                    p +=  _snprintf(p, 400, "%d: %s was hit by smash\n",get_elapsed_time(ev->time), dst->name);
                }
            }

        }

		/* common */
		cbtcount += 1;
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
