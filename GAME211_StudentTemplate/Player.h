#pragma once
#include "Entity.h"
#include <MMath.h>
#include <VMath.h>
#include <iostream>


#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DegToRad 0.0174533 //1 deg in rad

class Player: public Entity
{
  protected:
    int maxHealth;
    int currentHealth;
    int currentAmmo;
    float damageDelayTime; 
    float orientation;

  public:
    //stores values for controller
    int w, a, s, d;
    bool delayActive;
    Player();
    Player(int maxHealth_, int ammo_, float orientation_, Vec2 position_, Vec2 veloicty_);
    void playerUpdate(float deltaTime);
    float getOrientation()
    {
        return orientation;
    }
    void setOrientation(float orientation_) 
    {
        orientation = orientation_;
    }
    void addHealth(int addValue);
    void subHealth(int subValue);
    void addAmmo(int addValue);
    void subAmmo(int subValue);
    int getCurrentHealth();
    int getMaxHealth();
    int getAmmo();
};

