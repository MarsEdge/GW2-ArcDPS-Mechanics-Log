#include "imgui_panels.h"

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
void    ExampleAppLog::Clear()
{
    Buf.clear();
    LineOffsets.clear();
    reset_all_player_stats();
}

void    ExampleAppLog::AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
{
    int old_size = Buf.size();
    va_list args;
    va_start(args, fmt);
    Buf.appendv(fmt, args);
    va_end(args);
    for (int new_size = Buf.size(); old_size < new_size; old_size++)
        if (Buf[old_size] == '\n')
            LineOffsets.push_back(old_size);
    ScrollToBottom = true;
}

void    ExampleAppLog::Draw(const char* title, bool* p_open = NULL)
{
    ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiSetCond_FirstUseEver);
    ImGui::Begin(title, p_open);
    if (ImGui::Button("Clear")) Clear();
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);
    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (copy) ImGui::LogToClipboard();

    if (Filter.IsActive())
    {
        const char* buf_begin = Buf.begin();
        const char* line = buf_begin;
        for (int line_no = 0; line != NULL; line_no++)
        {
            const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
            if (Filter.PassFilter(line, line_end))
                ImGui::TextUnformatted(line, line_end);
            line = line_end && line_end[1] ? line_end + 1 : NULL;
        }
    }
    else
    {
        ImGui::TextUnformatted(Buf.begin());
    }

    if (ScrollToBottom)
        ImGui::SetScrollHere(1.0f);
    ScrollToBottom = false;
    ImGui::EndChild();
    ImGui::End();
}

void    AppChart::Draw(const char* title, std::vector<Player> players, bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiSetCond_FirstUseEver);
    ImGui::Begin(title, p_open);

    std::lock_guard<std::mutex> lg(players_mtx);

    size_t players_size = players.size();
    float column_width = ImGui::GetWindowContentRegionWidth()/5.0;

    ImGui::BeginGroup();
    ImGui::Text("Name");
    ImGui::SameLine(column_width*1);
    ImGui::Text("Received");
    ImGui::SameLine(column_width*2);
    ImGui::Text("Failed");
    ImGui::SameLine(column_width*3);
    ImGui::Text("Downs");
    ImGui::SameLine(column_width*4);
    ImGui::Text("Deaths");
    ImGui::EndGroup();

    for(uint16_t index = 0;index<players_size;index++)
    {
        if(players.at(index).is_relevant())
        {
            ImGui::BeginGroup();

            if(ImGui::CollapsingHeader(players.at(index).name.c_str()))
            {
                for(uint16_t tracker_index=0;
                tracker_index<players.at(index).tracker.size();
                tracker_index++)
                {
                    ImGui::BeginGroup();

                    ImGui::Text(players.at(index).tracker.at(tracker_index).name.c_str());
                    if(!players.at(index).tracker.at(tracker_index).fail)
                    {
                        ImGui::SameLine(column_width*1);
                    }
                    else
                    {
                        ImGui::SameLine(column_width*2);
                    }
                    ImGui::Text(std::to_string(players.at(index).tracker.at(tracker_index).hits).c_str());

                    ImGui::EndGroup();
                }
            }

                ImGui::Indent();
                ImGui::Text("Total");
                ImGui::SameLine(column_width*1);
                ImGui::Text(std::to_string(players.at(index).mechanics_received).c_str());

                ImGui::SameLine(column_width*2);
                ImGui::Text(std::to_string(players.at(index).mechanics_failed).c_str());
                ImGui::SameLine(column_width*3);
                ImGui::Text(std::to_string(players.at(index).downs).c_str());
                ImGui::SameLine(column_width*4);
                ImGui::Text(std::to_string(players.at(index).deaths).c_str());

                ImGui::EndGroup();
        }
    }

    ImGui::End();
}
