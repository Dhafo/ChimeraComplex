#include "Player.h"





Player::Player()
{
    maxHealth = 1;
    currentHealth = maxHealth;
    maxAmmo = 0;
    currentAmmo = 0;
    orientation = 0;
    position = Vec2(0,0);
    velocity = Vec2(0,0);

}

Player::Player(int maxhealth_,int maxAmmo_, float orientation_,Vec2 position_, Vec2 velocity_) {

    maxHealth= maxhealth_;
    currentHealth = maxHealth;
   maxAmmo = maxAmmo_;
    currentAmmo = 0;
   orientation = orientation_;
    position = position_;
    velocity = velocity_;


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
    currentHealth += subValue;
    if (currentHealth < 0) {

        currentHealth = maxHealth;
    }

}

void Player::addAmmo(int addValue)
{
    currentAmmo += addValue;
    if (currentAmmo > maxAmmo) {
        currentAmmo = maxAmmo;

    }
}

void Player::subAmmo(int subValue)
{
    currentAmmo -= subValue;
    if (currentAmmo > maxAmmo) {
        currentAmmo = maxAmmo;

    }
}



