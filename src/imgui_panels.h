#pragma once

#include "player.h"
#include "imgui.h"
#include <string>
#include <fstream>
#include <windows.h>
#include <shlobj.h>
#include <ctime>

struct AppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;

    void    Clear();
    void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2);

    void    Draw(const char* title, bool* p_open, ImGuiWindowFlags flags);
};

struct AppChart
{
    Player* merge_A;//1st player to merge
    Player* merge_B;//2nd player to merge
    ImGuiTextFilter     Filter;
    uint16_t last_export_total;
	std::string export_path;

    void    Clear();

    void    Draw(const char* title, bool* p_open, ImGuiWindowFlags flags, bool mods_pressed);

    std::string to_string();
    void    write_to_disk();
	std::string get_default_export_path();
};

float get_chart_column_width(float window_width);
float get_chart_column_loc(float window_width, uint16_t col);
