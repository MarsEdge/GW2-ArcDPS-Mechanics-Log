#pragma once
#include <vector>

#include "mechanics.h"

class Options
{
public:
	bool show_only_self = false;

	std::vector<Mechanic>* mechanics = nullptr;

	Options();
	~Options();
};

