#pragma once
#include "Entity.h"
#include <MMath.h>
#include <VMath.h>
class Item : public Entity
{
private:
    bool collected;
    int itemCountMax; 
public:
    Item( Vec2 position_, Vec2 velocity_);
};

