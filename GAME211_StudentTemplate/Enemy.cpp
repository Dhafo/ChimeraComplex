#include "Enemy.h"
#include <MMath.h>
using namespace std;
Enemy::Enemy() {

    maxHealth = 1;
    currentHealth = maxHealth;

    position = Vec2(0, 0);
    velocity = Vec2(0, 0);
    speed = 0.5;

}

 Enemy::Enemy(int maxHealth_, Vec2 position_, Vec2 velocity_) {

    maxHealth = maxHealth_;
    currentHealth = maxHealth;

    position = position_;
    velocity = velocity;


    speed = 0.5;

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

Vec2 Enemy::moveRatio(Vec2 otherPos)
{
    float angle;
    float xRatio;
    float yRatio;
    angle = atan2(otherPos.x - position.x, otherPos.y - position.y);
   // cout << "angle = " << angle << endl;

    xRatio = sin(angle);

   /* cout << "angle = " << xRatio << endl;*/
    yRatio = cos(angle);
  //  cout << "angle = " << yRatio << endl;
    return Vec2(xRatio,yRatio);
}

void Enemy::updatePos(Vec2 otherPos)
{
    position.x += moveRatio(otherPos).x*speed;
    position.y += moveRatio(otherPos).y*speed;


 /*   position.x += 1;
    position.y += 1;*/
}

