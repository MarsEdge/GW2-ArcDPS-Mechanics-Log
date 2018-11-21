#include "LogEvent.h"



LogEvent::LogEvent(Player* new_player, Mechanic* new_mechanic, uint64_t new_time, int64_t new_value)
{
	player = new_player;
	mechanic = new_mechanic;
	time = new_time;
	value = new_value;

	bakeTimeStr();
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
	
	output += time_str;

	output += " - ";

	output += player->name;

	output += " ";
	output += mechanic->name;

	if (value != 1)
	{
		output += ", value = " + std::to_string(value);
	}

	ImGui::TextUnformatted(output.c_str());
}

void LogEvent::bakeTimeStr()
{
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

	time_str = output;
}

std::string LogEvent::getFilterText()
{
	if (!player && !mechanic)//if placeholder event
	{
		return "=";
	}
	return time_str
		+ player->name
		+ mechanic->name;
}
