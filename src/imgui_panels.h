#pragma once

#include "imgui.h"

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
