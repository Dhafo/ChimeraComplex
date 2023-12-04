#include "Player.h"

using namespace std;

Player::Player()
{
    maxHealth = 1;
    currentHealth = maxHealth;
    currentAmmo = 0;
    orientation = 0;
    position = Vec2(0,0);
    velocity = Vec2(0,0);
    damageDelayTime = 0;
    delayActive = false;
}


Player::Player(int maxhealth_,int maxAmmo_, float orientation_,Vec2 position_, Vec2 velocity_)
{
    maxHealth= maxhealth_;
    currentHealth = maxHealth;
    currentAmmo = 7;
    orientation = orientation_;
    position = position_;
    velocity = velocity_;
    damageDelayTime = 0;
    delayActive = false;

    w = 0;
    a = 0;
    s = 0;
    d = 0;
}

void Player::playerUpdate(float deltaTime)
{
    if (damageDelayTime >0) 
    {
        damageDelayTime -= deltaTime;
    }
    else if (delayActive == true) 
    {
        delayActive = false;
    }
}


void Player::addHealth(int addValue)
{
    currentHealth += addValue;
    if (currentHealth > maxHealth) {

        currentHealth = maxHealth;
    }
}

void Player::subHealth(int subValue)
{
    if ( delayActive == false) 
    {
        cout << "player hit!" << endl;
        currentHealth -= subValue;
        delayActive = true;
        damageDelayTime = 2;
        cout << "player health = " << getCurrentHealth() << endl;
        if (currentHealth < 0) 
        {
            currentHealth = 0;
        }
    }
}

void Player::addAmmo(int addValue)
{
    currentAmmo += addValue;
}

void Player::subAmmo(int subValue)
{
    currentAmmo -= subValue;
    if (currentAmmo < 0)
    {
        currentAmmo = 0;
    }
}

int Player::getCurrentHealth()
{
    return currentHealth;
}

int Player::getMaxHealth()
{
    return maxHealth;
}

int Player::getAmmo()
{
    return currentAmmo;
}



