#pragma once
#include "Entity.h"
#include <MMath.h>
#include <VMath.h>
class Item : public Entity
{
    bool collected;
    
    void CreateItem( Vec2 position_, Vec2 veloicty_);

   

};

