#include "Entity.h"

Entity::Entity() 
{
    position = Vec2(0, 0);
    velocity = Vec2(0, 0);
    exist = true;
}

Entity::Entity(Vec2 position_, Vec2 velocity_, SDL_Texture* texture_)
{
    position = position_;
    velocity = velocity_;
    texture = texture_;
    exist = true;
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
    if (dist < 16) 
    {

        return true;
    }
    else
    {
        return false;
    } 
}

bool Entity::isCloser(Vec2 position1, Vec2 position2)
{
    //finds distance between first object and self
    float distPosX = position.x - position1.x; //entity pos x - player pos.x
    float distPosY = position.y - position1.y; //entity pos y - player pos.y
    float dist1 = sqrt((distPosX * distPosX) + (distPosY * distPosY));

    //finds distance between second object and self
    distPosX = position.x - position2.x;
    distPosY = position.y - position2.y;
    float dist2 = sqrt((distPosX * distPosX) + (distPosY * distPosY));

    //checks if object first is closer than second

    if (dist1 < dist2) {
        return true;
    }

    else {
        return false;
    }
    
}

float Entity::getDistance(Vec2 otherPosition)
{
    float result;
    float distPosX = position.x - otherPosition.x; //entity pos x - player pos.x
    float distPosY = position.y - otherPosition.y; //entity pos y - player pos.y
    result = sqrt((distPosX * distPosX) + (distPosY * distPosY));
    return result;
}


