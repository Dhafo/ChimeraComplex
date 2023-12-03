#ifndef ENTITY_H
#define ENTITY_H
#include <MMath.h>
#include <VMath.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace MATH;
//#include <Vector.h>
class Entity
{
protected:
    Vec2 velocity;
    Vec2 position;
    bool exist;
public:
    Entity();
    Entity(Vec2 position_, Vec2 velocity_, SDL_Texture* texture);
    void setPosition(Vec2 position_);
    Vec2 getPosition();
    void setvelocity(Vec2 velocity_);
    Vec2 getVelocity();
    bool collField(Vec2 Position_);
    SDL_Texture* texture;

    //checks if position 1 is closer than position 2
    bool isCloser(Vec2 position1,Vec2 position2);

    float getDistance(Vec2 otherPosition);

    bool getExist() 
    {
        return exist;
    }
    void setExist(bool exist_) 
    {
        exist = exist_;
    }
};

#endif /* ENTITY_H */
