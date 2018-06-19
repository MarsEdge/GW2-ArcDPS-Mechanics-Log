#include "imgui_panels.h"

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
void    AppLog::Clear()
{
    Buf.clear();
    LineOffsets.clear();
}

void    AppLog::AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
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

void    AppLog::Draw(const char* title, bool* p_open = NULL, ImGuiWindowFlags flags = 0)
{
    ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiSetCond_FirstUseEver);
	ImGui::Begin(title, p_open, flags);
	ImGui::PushAllowKeyboardFocus(false);
    if (ImGui::Button("Clear")) Clear();
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -50.0f);
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
	ImGui::PopAllowKeyboardFocus();
    ImGui::End();
}

void    AppChart::Clear()
{
    reset_all_player_stats();
}

void    AppChart::Draw(const char* title, bool* p_open, ImGuiWindowFlags flags, bool show_all)
{
    ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiSetCond_FirstUseEver);
    ImGui::Begin(title, p_open, flags);
	ImGui::PushAllowKeyboardFocus(false);

    float window_width = ImGui::GetWindowContentRegionWidth();
    bool expand = false;
    bool pop_color = false;
    ImVec4 merge_text_col = ImVec4(100,100,100,70);

    if (ImGui::Button("Clear")) Clear();
    ImGui::SameLine();
//  Filter.Draw("Filter", -50.0f);
//  ImGui::SameLine();
    if (ImGui::Button("Export")) write_to_disk();
    ImGui::SameLine();
    if (ImGui::Button("Copy")) ImGui::LogToClipboard();
    ImGui::Separator();
    bool merge = merge_A && merge_B && merge_A->id != merge_B->id;
    if(merge)
    {
		merge_A->merge(merge_B);
    }

    ImGui::BeginGroup();
    ImGui::Text("Name");
    ImGui::SameLine(get_chart_column_loc(window_width,1));
    ImGui::Text("Received");
    ImGui::SameLine(get_chart_column_loc(window_width,2));
    ImGui::Text("Failed");
    ImGui::SameLine(get_chart_column_loc(window_width,3));
    ImGui::Text("Downs");
    ImGui::SameLine(get_chart_column_loc(window_width,4));
    ImGui::Text("Deaths");
    ImGui::SameLine(get_chart_column_loc(window_width,5));
    ImGui::Text("Pulls");
    ImGui::SameLine(get_chart_column_loc(window_width,6));
    ImGui::Text("Merge");
    ImGui::SameLine(get_chart_column_loc(window_width,7));
    ImGui::Text("Delete");
    ImGui::EndGroup();

    ImGui::BeginChild("scrolling");
    for(uint16_t index = 0;index<players.size();index++)
    {
        if(players.at(index).is_relevant())
        {
            ImGui::PushItemWidth(window_width*0.9);
            ImGui::AlignFirstTextHeightToWidgets();
            expand = ImGui::TreeNode(players.at(index).name.c_str());

            ImGui::SameLine(get_chart_column_loc(window_width,1));
            ImGui::Text("%d", players.at(index).mechanics_received);
            ImGui::SameLine(get_chart_column_loc(window_width,2));
            ImGui::Text("%d", players.at(index).mechanics_failed);
            ImGui::SameLine(get_chart_column_loc(window_width,3));
            ImGui::Text("%d", players.at(index).downs);
            ImGui::SameLine(get_chart_column_loc(window_width,4));
            ImGui::Text("%d", players.at(index).deaths);
            ImGui::SameLine(get_chart_column_loc(window_width,5));
            ImGui::Text("%d", players.at(index).pulls);
            ImGui::PopItemWidth();
            ImGui::SameLine(get_chart_column_loc(window_width,6));
            if(merge_A
               && merge_A->id == players.at(index).id)
            {
                ImGui::PushStyleColor(ImGuiCol_Text,merge_text_col);
                pop_color = true;
            }
            if(ImGui::SmallButton("To"))
            {
                if(merge_A
                   && merge_A->id == players.at(index).id)
                {
                    merge_A = nullptr;
                }
                else
                {
                    merge_A = &players.at(index);
                }
            }
            if(pop_color)
            {
                ImGui::PopStyleColor();
                pop_color = false;
            }
            ImGui::SameLine();
            if(merge_B
               && merge_B->id == players.at(index).id)
            {
                ImGui::PushStyleColor(ImGuiCol_Text,merge_text_col);
                pop_color = true;
            }

            if(ImGui::SmallButton("From"))
            {
                if(merge_B
                   && merge_B->id == players.at(index).id)
                {
                    merge_B = nullptr;
                }
                else
                {
                    merge_B = &players.at(index);
                }
            }
            if(pop_color)
            {
                ImGui::PopStyleColor();
                pop_color = false;
            }
            ImGui::SameLine(get_chart_column_loc(window_width,7));
            if(ImGui::SmallButton("X")
               || (merge
                   && players.at(index).id == merge_B->id))
            {
                if(expand)
                {
                    ImGui::TreePop();
                }
                continue;
            }

            if(expand
				|| show_all)
            {
                for(uint16_t tracker_index=0;
                tracker_index<players.at(index).tracker.size();
                tracker_index++)
                {
                    ImGui::PushItemWidth(window_width*0.9);
                    ImGui::Indent();
                    ImGui::Text(players.at(index).tracker.at(tracker_index).name.c_str());
                    if(!players.at(index).tracker.at(tracker_index).fail)
                    {
                        ImGui::SameLine(get_chart_column_loc(window_width,1));
                    }
                    else
                    {
                        ImGui::SameLine(get_chart_column_loc(window_width,2));
                    }
                    ImGui::Text("%d", players.at(index).tracker.at(tracker_index).hits);
                    ImGui::SameLine(get_chart_column_loc(window_width,5));
                    ImGui::Text("%d", players.at(index).tracker.at(tracker_index).pulls);
                    ImGui::Unindent();
                    ImGui::PopItemWidth();

                    ImGui::Separator();
                }

				if (expand) ImGui::TreePop();
            }
            ImGui::Separator();
        }
    }
    ImGui::EndChild();
	ImGui::PopAllowKeyboardFocus();
    ImGui::End();

    if(merge || merge_A == merge_B)
    {
        merge_A = nullptr;
        merge_B = nullptr;
    }
}

float get_chart_column_width(float window_width)
{
    return window_width/6.0*3.0/5.0;
}

float get_chart_column_loc(float window_width, uint16_t col)
{
     return (window_width/6.0) + col * get_chart_column_width(window_width);
}

std::string AppChart::to_string()
{
    std::string output = "";

    output += "Player Name,Account Name,Mechanic Name,Received,Failed,Downs,Deaths,Pulls\n";

    for(uint16_t index=0;index<players.size();index++)
    {
        if(players.at(index).is_relevant())
        {
            output += players.at(index).to_string();
        }
    }
    return output;
}

void AppChart::write_to_disk()
{
    uint16_t new_export_total = get_mechanics_total();
    if(last_export_total == new_export_total)
    {
        return;
    }

	std::string text = to_string();

    std::time_t t = std::time(nullptr);
    char time_str[100];
    if (std::strftime(time_str, sizeof(time_str), "%Y%m%d-%H%M%S", std::localtime(&t)))
    {
        //std::cout << time_str << '\n';
    }

    CreateDirectory(export_path.c_str(), NULL);

    std::ofstream out(export_path+"\\"+std::string(time_str)+"-"+std::to_string(new_export_total)+".csv");
    out << text;
    out.close();

    last_export_total = new_export_total;
}

std::string AppChart::get_default_export_path()
{
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	if (result != S_OK)
	{
		//std::cout << "Error: " << result << "\n";
	}
	else
	{
		return std::string(my_documents) + "\\Guild Wars 2\\addons\\arcdps\\arcdps.mechanics";
	}
	return "";
}