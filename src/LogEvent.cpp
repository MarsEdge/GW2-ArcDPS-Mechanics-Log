#include "LogEvent.h"



LogEvent::LogEvent(Player* new_player, Mechanic* new_mechanic, uint64_t new_time, int64_t new_value)
{
	player = new_player;
	mechanic = new_mechanic;
	time = new_time;
	value = new_value;
}


LogEvent::~LogEvent()
{
}

void LogEvent::draw()
{
	if (!player && !mechanic)//if placeholder event
	{
		ImGui::Separator();//TODO: make function for if an event is a placeholder
		return;
	}
	
	std::string output = "";
	if (time < 0)
	{
		output += "-";
	}
	output += std::to_string(abs(time / 60));
	output += ":";
	if (abs(time) % 60 < 10)
	{
		output += "0";
	}
	output += std::to_string(abs(time) % 60);
	output += " - ";

	output += player->name;

	output += " ";
	output += mechanic->name;

	if (value != 1)
	{
		output += ", value = " + std::to_string(value);
	}

	ImGui::Text(output.c_str());
}

std::string LogEvent::getFilterText()
{
	if (!player && !mechanic)//if placeholder event
	{
		return "=";
	}
	return std::to_string(time)
		+ player->name
		+ mechanic->name;
}
