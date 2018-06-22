#include "imgui_panels.h"

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
void    AppLog::clear()
{
    buf.clear();
    line_offsets.clear();
}

void    AppLog::addLog(const char* fmt, ...) IM_PRINTFARGS(2)
{
    int old_size = buf.size();
    va_list args;
    va_start(args, fmt);
    buf.appendv(fmt, args);
    va_end(args);
    for (int new_size = buf.size(); old_size < new_size; old_size++)
        if (buf[old_size] == '\n')
            line_offsets.push_back(old_size);
    scroll_to_bottom = true;
}

void    AppLog::draw(const char* title, bool* p_open = NULL, ImGuiWindowFlags flags = 0)
{
    ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiSetCond_FirstUseEver);
	ImGui::Begin(title, p_open, flags);
	ImGui::PushAllowKeyboardFocus(false);
    if (ImGui::Button("Clear")) clear();
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    filter.Draw("Filter", -50.0f);
    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (copy) ImGui::LogToClipboard();

    if (filter.IsActive())
    {
        const char* buf_begin = buf.begin();
        const char* line = buf_begin;
        for (int line_no = 0; line != NULL; line_no++)
        {
            const char* line_end = (line_no < line_offsets.Size) ? buf_begin + line_offsets[line_no] : NULL;
            if (filter.PassFilter(line, line_end))
                ImGui::TextUnformatted(line, line_end);
            line = line_end && line_end[1] ? line_end + 1 : NULL;
        }
    }
    else
    {
        ImGui::TextUnformatted(buf.begin());
    }

    if (scroll_to_bottom)
        ImGui::SetScrollHere(1.0f);
	scroll_to_bottom = false;
    ImGui::EndChild();
	ImGui::PopAllowKeyboardFocus();
    ImGui::End();
}

void    AppChart::clear()
{
    resetAllPlayerStats();
}

void    AppChart::draw(const char* title, bool* p_open, ImGuiWindowFlags flags, bool show_all)
{
    ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiSetCond_FirstUseEver);
    ImGui::Begin(title, p_open, flags);
	ImGui::PushAllowKeyboardFocus(false);

    float window_width = ImGui::GetWindowContentRegionWidth();
    bool expand = false;
    bool pop_color = false;
    ImVec4 merge_text_col = ImVec4(100,100,100,70);

    if (ImGui::Button("Clear")) clear();
    ImGui::SameLine();
//  Filter.Draw("Filter", -50.0f);
//  ImGui::SameLine();
    if (ImGui::Button("Export")) writeToDisk();
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
    ImGui::SameLine(getChartColumnLoc(window_width,1));
    ImGui::Text("Received");
    ImGui::SameLine(getChartColumnLoc(window_width,2));
    ImGui::Text("Failed");
    ImGui::SameLine(getChartColumnLoc(window_width,3));
    ImGui::Text("Downs");
    ImGui::SameLine(getChartColumnLoc(window_width,4));
    ImGui::Text("Deaths");
    ImGui::SameLine(getChartColumnLoc(window_width,5));
    ImGui::Text("Pulls");
    ImGui::SameLine(getChartColumnLoc(window_width,6));
    ImGui::Text("Merge");
    ImGui::SameLine(getChartColumnLoc(window_width,7));
    ImGui::Text("Delete");
    ImGui::EndGroup();

    ImGui::BeginChild("scrolling");
    for(uint16_t index = 0;index<players.size();index++)
    {
        if(players.at(index).isRelevant())
        {
            ImGui::PushItemWidth(window_width*0.9);
            ImGui::AlignFirstTextHeightToWidgets();
            expand = ImGui::TreeNode(players.at(index).name.c_str());

            ImGui::SameLine(getChartColumnLoc(window_width,1));
            ImGui::Text("%d", players.at(index).mechanics_received);
            ImGui::SameLine(getChartColumnLoc(window_width,2));
            ImGui::Text("%d", players.at(index).mechanics_failed);
            ImGui::SameLine(getChartColumnLoc(window_width,3));
            ImGui::Text("%d", players.at(index).downs);
            ImGui::SameLine(getChartColumnLoc(window_width,4));
            ImGui::Text("%d", players.at(index).deaths);
            ImGui::SameLine(getChartColumnLoc(window_width,5));
            ImGui::Text("%d", players.at(index).pulls);
            ImGui::PopItemWidth();
            ImGui::SameLine(getChartColumnLoc(window_width,6));
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
            ImGui::SameLine(getChartColumnLoc(window_width,7));
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
                        ImGui::SameLine(getChartColumnLoc(window_width,1));
                    }
                    else
                    {
                        ImGui::SameLine(getChartColumnLoc(window_width,2));
                    }
                    ImGui::Text("%d", players.at(index).tracker.at(tracker_index).hits);
                    ImGui::SameLine(getChartColumnLoc(window_width,5));
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

float getChartColumnLoc(float window_width, uint16_t col)
{
     return (window_width/6.0) + col * get_chart_column_width(window_width);
}

std::string AppChart::toString()
{
    std::string output = "";

    output += "Player Name,Account Name,Mechanic Name,Received,Failed,Downs,Deaths,Pulls\n";

    for(uint16_t index=0;index<players.size();index++)
    {
        if(players.at(index).isRelevant())
        {
            output += players.at(index).toString();
        }
    }
    return output;
}

void AppChart::writeToDisk()
{
    uint16_t new_export_total = getMechanicsTotal();
    if(last_export_total == new_export_total)
    {
        return;
    }

	std::string text = toString();

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

std::string AppChart::getDefaultExportPath()
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