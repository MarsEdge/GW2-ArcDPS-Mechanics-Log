#pragma once

#include "imgui.h"
#include "PlayerEntry.h"
#include "MechanicEntry.h"
#include "LogEvent.h"
#include "npc_ids.h"
#include <string>

class MechanicFilter
{
public:

	ImGuiTextFilter filter_player;
	ImGuiTextFilter filter_boss;
	ImGuiTextFilter filter_mechanic;

	bool show_in_squad_only = false;

	MechanicFilter();
	~MechanicFilter();

	void drawPopup();
	bool isActive();
	bool passFilter(Player* new_player, Boss* new_boss, Mechanic* new_mechanic);
	bool passFilter(LogEvent* new_event);
};

