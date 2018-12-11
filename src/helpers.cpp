#include "helpers.h"

bool isPlayer(const ag* new_player) noexcept
{
	return new_player
		&& new_player->elite != 0xffffffff
		&& new_player->name
		&& new_player->id;
}
