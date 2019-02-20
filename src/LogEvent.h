#pragma once

#include "imgui.h"

#include "mechanics.h"
#include "player.h"

class LogEvent
{
public:

	Player* player = nullptr;
	Mechanic* mechanic = nullptr;
	int time = 0;
	std::string str = "";
	int64_t value = 1;

	LogEvent(Player* new_player, Mechanic* new_mechanic, uint64_t new_time, int64_t new_value);

	void draw();
	void bakeStr();
	std::string getFilterText();
};

