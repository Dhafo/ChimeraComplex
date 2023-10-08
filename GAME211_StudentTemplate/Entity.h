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
private:

    Entity();
    Entity(Vec2 position_, Vec2 velocity_);

    void setPosition(Vec2 position_);
    Vec2 getPositon();

    void setvelocity(Vec2 velocity_);

    Vec2 getVeloicty();

    bool collField(Vec3 Position_);
};

#endif /* ENTITY_H */
