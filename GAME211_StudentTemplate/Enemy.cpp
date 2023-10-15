#include "Enemy.h"


 Enemy::Enemy(int maxHealth_, Vec2 position_, Vec2 velocity_) {

    maxHealth = maxHealth_;
    currentHealth = maxHealth;

    position = position_;
    velocity = velocity;




}



void Enemy::addHealth(int addValue)
{
    currentHealth += addValue;
    if (currentHealth > maxHealth) {

        currentHealth = maxHealth;
    }


}

void Enemy::subtractHealth(int subValue)
{
    currentHealth += subValue;
    if (currentHealth < 0) {

        currentHealth = maxHealth;
    }


}

