#pragma once

#include "player.h"
#include "imgui.h"
#include "Tracker.h"
#include "Options.h"
#include <string>
#include <fstream>
#include <windows.h>
#include <shlobj.h>
#include <ctime>

struct AppLog
{
    ImGuiTextBuffer     buf;
    ImGuiTextFilter     filter;
    ImVector<int>       line_offsets;        // Index to lines offset
    bool                scroll_to_bottom;

    void    clear();
    void    addLog(const char* fmt, ...) IM_PRINTFARGS(2);

    void    draw(const char* title, bool* p_open, ImGuiWindowFlags flags);
};

struct AppChart
{
    Player* merge_A;//1st player to merge
    Player* merge_B;//2nd player to merge
    ImGuiTextFilter     filter;
    uint16_t last_export_total;
	std::string export_path;

    void    clear(Tracker* tracker);

    void    draw(Tracker* tracker, const char* title, bool* p_open, ImGuiWindowFlags flags, bool show_all);

    std::string toString(Tracker* tracker);
    void    writeToDisk(Tracker* tracker);
	std::string getDefaultExportPath();
};

struct AppOptions
{
	void    draw(Options* options, const char* title, bool* p_open, ImGuiWindowFlags flags);
};

float getChartColumnWidth(float window_width);
float getChartColumnLoc(float window_width, uint16_t col);
