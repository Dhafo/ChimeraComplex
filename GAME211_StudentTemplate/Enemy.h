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
    Enemy(int maxhealth_, Vec2 position_, Vec2 velocity_, SDL_Texture* texture_) ;

   // void CreateEnemy(int maxhealth_, Vec2 position_, Vec2 velocity_);
    
    void addHealth(int addValue);

    void subtractHealth(int subValue);
    // gets angle towards a given postion and returns the cos/sin so we can move towards that direction
    Vec2 moveRatio(Vec2 otherPos);

    void updatePos(Vec2 otherPos);

    bool predMoveCheck(Vec2 pointA, Vec2 pointB, Vec2 pointC);

    bool VisionCheck(Player player, float visionRange);

    float getOrientation() {

        return orientation;

    }
    void setOrientation(float orientation_) {

        orientation = orientation_;
    }
};



#endif /* Enemy_H */
