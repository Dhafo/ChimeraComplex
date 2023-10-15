#ifndef Enemy_H
#define Enemy_H
#include "Entity.h"
#include <MMath.h>
#include <VMath.h>
class Enemy : public Entity
{

    int maxHealth;
    float orientation;
    int currentHealth;
    
  

    Enemy();
    Enemy(int maxhealth_, Vec2 position_, Vec2 velocity_) ;

   // void CreateEnemy(int maxhealth_, Vec2 position_, Vec2 velocity_);
    
    void addHealth(int addValue);

    void subtractHealth(int subValue);



};

#endif /* Enemy_H */
