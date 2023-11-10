#include "Enemy.h"
#include <MMath.h>
#include "Player.h"

using namespace std;
Enemy::Enemy() {

    maxHealth = 1;
    currentHealth = maxHealth;

    position = Vec2(0, 0);
    velocity = Vec2(0, 0);
    speed = 0.5;
    exist = true;
}

 Enemy::Enemy(int maxHealth_, Vec2 position_, Vec2 velocity_, SDL_Texture* texture_) {

    maxHealth = maxHealth_;
    currentHealth = maxHealth;

    position = position_;
    velocity = velocity;
    texture = texture_;

    speed = 0.5;
    exist = true;
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
    currentHealth -= subValue;
    if (currentHealth < 0) {

        currentHealth = maxHealth;
    }
    cout << "enemy hit" << endl;

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


bool Enemy::VisionCheck(Player player, float visionRange)
{

    //first find distance between enemy and player
    int distPosX = position.x - player.getPosition().x; //enemy pos x - player pos.x
    int distPosY = position.y - player.getPosition().y; //enemy pos y - player pos.y

    int dist = sqrt((distPosX * distPosX) + (distPosY * distPosY));

    //then angle (also considering pos y is downward)
    float angle = atan2(-distPosY, distPosX); //p = atan2(-h.y, h.x) * (180/PI)


    angle *= RADIANS_TO_DEGREES;

    //correct angle if it's out of bounds
    if (angle > 360)
    {
        angle -= 360;
    }
    else if (angle < 0)
    {
        angle += 360;
    }

    //  cout << angle << endl;

    float yTemp = (player.getOrientation() * RADIANS_TO_DEGREES) - angle;

    //  cout << yTemp << endl;



    if (yTemp > -visionRange && yTemp < visionRange) {
      
        return true;
    }
    else {
        return false;
    }
    

}

