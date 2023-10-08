#include "Entity.h"

Entity::Entity() {

    position = Vec2(0, 0);
    velocity = Vec2(0, 0);

}

Entity::Entity(Vec2 position_, Vec2 velocity_) {
    position = position_;
    velocity = velocity_;


}

void Entity::setPosition(Vec2 position_)
{
    position = position_;


}

Vec2 Entity::getPositon()
{
    return position;
}

void Entity::setvelocity(Vec2 velocity_)
{
    velocity = velocity_;

}

Vec2 Entity::getVeloicty()
{
    return Vec2(velocity);
}

bool Entity::collField(Vec3 position)
{

    //finds distance between other object and self
    int distPosX = (4 * 64) - position.x; //entity pos x - player pos.x
    int distPosY = (4 * 64) - position.y; //entity pos y - player pos.y


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
