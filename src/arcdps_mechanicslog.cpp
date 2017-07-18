/*
* arcdps combat api example
*/

#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include "mechanic_ids.h"

/* arcdps export table */
typedef struct arcdps_exports {
	uintptr_t ext_size; /* arcdps internal use, ignore */
	uintptr_t ext_sig; /* pick a number between 0 and uint64_t max that isn't used by other modules */
	void* ext_wnd; /* wndproc callback, fn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) */
	void* ext_combat; /* combat event callback, fn(cbtevent* ev, ag* src, ag* dst, char* skillname) */
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
	arc_exports.ext_sig = 0x81004122;
	arc_exports.ext_wnd = mod_wnd;
	arc_exports.ext_combat = mod_combat;
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
		if (ev->is_statechange) {
		}

		/* activation */
		else if (ev->is_activation) {
		}

		/* buff remove */
		else if (ev->is_buffremove) {
		}

		/* buff */
		else if (ev->buff) {
		}

		/* physical */
		else {
			if(ev->dst_agent) {
                //if attack hits (not evaded/invuln)
                if(ev->result==0){
                    //vg teleport
                    if(ev->skillid==MECHANIC_VG_GREEN_TELEPORT) {
                        p +=  _snprintf(p, 400, "%llu: %s was teleported\n",ev->time, dst->name);
                    }
                    //deimos oil
                    if(ev->skillid==MECHANIC_DEIMOS_OIL) {
                        p +=  _snprintf(p, 400, "%llu: %s touched an oil\n",ev->time, dst->name);
                    }
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
