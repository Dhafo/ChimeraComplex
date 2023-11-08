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

public:

    Entity();
    Entity(Vec2 position_, Vec2 velocity_, SDL_Texture* texture);

    void setPosition(Vec2 position_);
    Vec2 getPosition();

    void setvelocity(Vec2 velocity_);

    Vec2 getVelocity();

    bool collField(Vec2 Position_);
    SDL_Texture* texture;
};

#endif /* ENTITY_H */
