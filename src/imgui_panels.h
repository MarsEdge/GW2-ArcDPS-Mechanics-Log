#pragma once

#include "player.h"
#include "imgui.h"
#include <string>

struct ExampleAppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;

    void    Clear();
    void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2);

    void    Draw(const char* title, bool* p_open);
};

struct AppChart
{
    void    Clear();

    void    Draw(const char* title, std::vector<Player> players, bool* p_open);
};

float get_chart_column_width(float window_width);
float get_chart_column_loc(float window_width, uint16_t col);
