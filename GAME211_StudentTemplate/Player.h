#pragma once
#include "Entity.h"
#include <MMath.h>
#include <VMath.h>
class Player: public Entity
{
protected:
    int maxHealth;
    int currentHealth;
    int maxAmmo;
    int currentAmmo;

    float orientation;

  public:
    Player();

    Player(int maxHealth_, int ammo_, float orientation_, Vec2 position_, Vec2 veloicty_);


   /* void CreatePlayer(int maxHealth_, int ammo_, float orientation_, Vec2 position_, Vec2 veloicty_);*/

    float getOrientation() {

        return orientation;
    }
    void setOrientation(float orientation_) {

        orientation = orientation_;
    }

  
    void addHealth(int addValue);
    void subHealth(int subValue);
    void addAmmo(int addValue);
    void subAmmo(int subValue);
   






};

