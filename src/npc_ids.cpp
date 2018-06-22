#include "npc_ids.h"

Boss::Boss()
{
    ids = std::vector<uint32_t>();
    name = "";
    timer = 0;
    health = 0;
	pulls = 0;
}

bool Boss::hasId(uint32_t new_id)
{
    for(uint16_t index=0;index<ids.size();index++)
    {
        if(new_id == ids.at(index))
        {
            return true;
        }
    }
    return false;
}

