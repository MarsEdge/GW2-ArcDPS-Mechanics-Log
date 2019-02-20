#pragma once

#include "imgui.h"

#include "mechanics.h"
#include "player.h"

class LogEvent
{
public:

	Player* player = nullptr;
	Mechanic* mechanic = nullptr;
	int time = 0;//time from starting the pull to the event happening
	uint64_t time_absolute = 0;//ev->time at the instance of event
	std::string str = "";
	int64_t value = 1;

	LogEvent(Player* new_player, Mechanic* new_mechanic, uint64_t new_time, uint64_t new_time_absolute, int64_t new_value);

	void draw();
	void bakeStr();
	std::string getFilterText();
};

