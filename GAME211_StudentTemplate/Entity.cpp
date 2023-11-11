#include "Entity.h"

Entity::Entity() {

    position = Vec2(0, 0);
    velocity = Vec2(0, 0);
    exist = true;
}

Entity::Entity(Vec2 position_, Vec2 velocity_, SDL_Texture* texture_) {

    position = position_;
    velocity = velocity_;
<<<<<<< HEAD
    texture = texture_;
=======
<<<<<<< Updated upstream


=======
    texture = texture_;
    exist = true;
>>>>>>> Stashed changes
>>>>>>> Ramy
}

void Entity::setPosition(Vec2 position_)
{
    position = position_;


}

Vec2 Entity::getPosition()
{
    return position;
}

void Entity::setvelocity(Vec2 velocity_)
{
    velocity = velocity_;

}

Vec2 Entity::getVelocity()
{
    return Vec2(velocity);
}

bool Entity::collField(Vec2 position_)
{

    //finds distance between other object and self
    int distPosX = position.x - position_.x; //entity pos x - player pos.x
    int distPosY = position.y - position_.y; //entity pos y - player pos.y


    int dist = sqrt((distPosX * distPosX) + (distPosY * distPosY));

    // checks if distance is less than collision radius

    if (dist < 16) {

        return true;
    }
    else
    {
        return false;
    }
    
}


