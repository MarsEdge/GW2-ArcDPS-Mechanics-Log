#include <Windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include "simpleini\SimpleIni.h"

#include "imgui.h"
#include "imgui_panels.h"
#include "arcdps_datastructures.h"
#include "helpers.h"
#include "mechanics.h"
#include "player.h"
#include "skill_ids.h"
#include "npc_ids.h"

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
static int change_export_path(ImGuiTextEditCallbackData *data);
void parse_ini();
void write_ini();

int64_t start_time = 0;

std::string print_buffer = "";
bool show_app_log;

bool show_app_chart;
AppChart chart;

game_state gs;

CSimpleIniA ini(true);
bool valid_ini = false;


inline int get_elapsed_time(uint64_t &current_time)
{
    if(gs.boss_found
       && gs.boss_data.timer)
    {
        return (gs.boss_data.timer-(static_cast<int64_t>(current_time)-start_time))/1000;
    }
    return (current_time-start_time)/1000;
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
	ImGui::SetCurrentContext((ImGuiContext*)imguicontext);
	return mod_init;
}

/* export -- arcdps looks for this exported function and calls the address it returns */
extern "C" __declspec(dllexport) void* get_release_addr() {
	arcvers = 0;
	return mod_release;
}

/* initialize mod -- return table that arcdps will use for callbacks */
arcdps_exports* mod_init()
{
	/* for arcdps */
	arc_exports.size = sizeof(arcdps_exports);
	arc_exports.out_name = "Mechanics Log";
	arc_exports.out_build = "0.3";
	arc_exports.sig = 0x81004122;//from random.org
	arc_exports.wnd = mod_wnd;
	arc_exports.combat = mod_combat;
	arc_exports.imgui = mod_imgui;
	arc_exports.options = mod_options;

	parse_ini();

	return &arc_exports;
}

/* release mod -- return ignored */
uintptr_t mod_release()
{
    chart.write_to_disk(players);
    players.clear();
	write_ini();
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
uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname)
{
	Player* current_player = nullptr;
	std::string output = "";

	/* ev is null. dst will only be valid on tracking add. skillname will also be null */
	if (!ev)
    {
        /* notify tracking change */
		if (!src->elite)
        {
			/* add */
			if (src->prof)
            {
                add_player(src->name,dst->name,src->id);
			}

			/* remove */
			else
            {
                remove_player(src->name, src->name, src->id);
			}
		}
	}

	/* combat event. skillname may be null. non-null skillname will remain static until module is unloaded. refer to evtc notes for complete detail */
	else
    {

		/* common */

		/* statechange */
		if (ev->is_statechange)
        {
            if(ev->is_statechange==1)
            {
                if(src->self)
                {
                    start_time = ev->time;
                    if(has_logged_mechanic)
                    {
                        has_logged_mechanic = false;
                        output += "===========\n";
                    }
                }
                else
                {
                    
                }
            }

            else if(ev->is_statechange==2)
            {

            }

            //if rally
            else if(ev->is_statechange==3)
            {
                current_player = get_player(src);
                if(current_player)
                {
                    current_player->rally();
                }
            }

            //if dead
            else if(ev->is_statechange==4)
            {
                current_player = get_player(src);
                if(current_player)
                {
                    current_player->dead();
                }
            }

            //if downed
            else if(ev->is_statechange==5)
            {
                current_player = get_player(src);
                if(current_player)
                {
                    current_player->down();
                }
            }
            //if health update
            else if(ev->is_statechange==12)
            {
				
            }
		}

		/* activation */
		else if (ev->is_activation)
        {

		}

		/* buff remove */
		else if (ev->is_buffremove)
        {
            if (ev->skillid==BUFF_STABILITY)//if it's stability
            {
                current_player=get_player(dst);
                if(current_player)
                {
                    current_player->set_stab_time(ev->time+ms_per_tick);//cut the ending time of stab early
                }
            }
            else if (ev->skillid==BUFF_VAPOR_FORM//vapor form manual case
                     || ev->skillid==BUFF_ILLUSION_OF_LIFE//Illusion of Life manual case
                     )
            {
                current_player=get_player(dst);
                if(current_player)
                {
                    current_player->fix_double_down();
                }
            }

		}

		/* buff */
		else if (ev->buff)
        {
            if (ev->skillid==BUFF_STABILITY)//if it's stability
            {
                current_player=get_player(dst);
                if(current_player)
                {
                    current_player->set_stab_time(ev->time+ev->value+ms_per_tick);//add prediction of when new stab will end
                }
            }
		}

        if(ev->dst_agent)
        {
            //if attack hits (not block/evaded/invuln/miss)
            //and it's a player, not a summon
            if(ev->result==0 || ev->result==1 || ev->result==2 || ev->result==8)
            {

                for(uint16_t index=0;index<mechanics.size();index++)
                {
                    if(mechanics[index].is_valid_hit(ev, src, dst, &gs))
                    {
                        int time = get_elapsed_time(ev->time);
                        if(time < 0)
                        {
                            output += "-";
                        }
                        output += std::to_string(abs(time/60));
                        output += ":";
                        if(abs(time)%60 < 10)
                        {
                            output += "0";
                        }
                        output += std::to_string(abs(time)%60);
                        output += " - ";
                        if(mechanics[index].target_is_dst)
                        {
                            output += dst->name;
                        }
                        else
                        {
                            output += src->name;
                        }
                        output += " ";
                        output += mechanics[index].name;
                        output += "\n";
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
            output += "\n";
        }
	}

    print_buffer +=output;
	return 0;
}

void ShowMechanicsLog(bool* p_open)
{
    static AppLog log;

    if(print_buffer.size() > 0)
    {
        log.AddLog(print_buffer.c_str());
        print_buffer = "";
    }

    if(show_app_log) log.Draw("MECHANICS LOG", p_open);
}

void ShowMechanicsChart(bool* p_open)
{
    if(show_app_chart) chart.Draw("MECHANICS CHART", players, p_open);
}

uintptr_t mod_imgui()
{
    ShowMechanicsLog(&show_app_log);

    ShowMechanicsChart(&show_app_chart);

    return 0;
}

uintptr_t mod_options()
{
    ImGui::Checkbox("MECHANICS LOG", &show_app_log);
    ImGui::Checkbox("MECHANICS CHART", &show_app_chart);

    return 0;
}

static int change_export_path(ImGuiTextEditCallbackData *data)
{
	chart.export_path = data->Buf;
}

void parse_ini()
{
	SI_Error rc = ini.LoadFile("addons\\arcdps\\arcdps_mechanics.ini");
	valid_ini = rc < 0;

	std::string pszValue = ini.GetValue("log","show", "0");
	show_app_log = std::stoi(pszValue);

	pszValue = ini.GetValue("chart", "show", "0");
	show_app_chart = std::stoi(pszValue);

	pszValue = ini.GetValue("chart", "export_path", chart.get_default_export_path().c_str());
	chart.export_path = pszValue;
}

void write_ini()
{
	SI_Error rc = ini.SetValue("log", "show", std::to_string(show_app_log).c_str());
	rc = ini.SetValue("chart", "show", std::to_string(show_app_chart).c_str());
	rc = ini.SetValue("chart", "export_path", chart.export_path.c_str());

	rc = ini.SaveFile("addons\\arcdps\\arcdps_mechanics.ini");
}