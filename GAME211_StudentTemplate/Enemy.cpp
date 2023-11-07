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

bool Enemy::predMoveCheck(Vec2 pointA, Vec2 pointB, Vec2 pointC)
{
    Vec2 triBCCResult;

    float detT = (pointB.y - pointC.y) * (pointA.x - pointC.x) +
                 (pointC.x - pointB.x) * (pointA.y - pointC.y);
    
    triBCCResult.x = ((pointB.y - pointC.y) * (position.x - pointC.x) +
                (pointC.x - pointB.x) * (position.y - pointC.y)) / detT;
    
    triBCCResult.y = ((pointC.y - pointA.y) * (position.x - pointC.x) +
                (pointA.x - pointC.x) * (position.y - pointC.y)) / detT;
    
     triBCCResult;
     
     if (triBCCResult.x >= 0.0f && triBCCResult.y >= 0.0f && (triBCCResult.x + triBCCResult.y) <= 1.0f) {
         return true;
     }
     else
     {
         return false;
     }
}

