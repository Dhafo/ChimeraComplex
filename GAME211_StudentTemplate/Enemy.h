#ifndef Enemy_H
#define Enemy_H
#include "Entity.h"
#include"Player.h"
#include <MMath.h>
#include <VMath.h>
#include <iostream>
class Enemy : public Entity
{
protected:
    int maxHealth;
    float orientation;
    int currentHealth;
    float speed;


public:
    Enemy();
    Enemy(int maxhealth_, Vec2 position_, Vec2 velocity_, SDL_Texture* texture_);

  

    void addHealth(int addValue);

    void subtractHealth(int subValue);
    // gets angle towards a given postion and returns the cos/sin so we can move towards that direction
    Vec2 moveRatio(Vec2 otherPos);

    void updatePos(Vec2 otherPos);

   
    //checks player orintation and if the enemy is within the angel of vision. The vision range value is actaully double the amount because it checks bot negative and positive
    bool VisionCheck(Player player, float visionRange);

    int getHealth() {

        return currentHealth;
    }

    float getOrientation() {

        return orientation;

    }
    void setOrientation(float orientation_) {

        orientation = orientation_;
    }
};



#endif /* Enemy_H */