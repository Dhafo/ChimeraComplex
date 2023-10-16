#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Vector.h"
#include "Scene.h"


using namespace std;
using namespace MATH;

class Button
{
private:
    SDL_Surface* image;
    SDL_Texture* texture;
    SDL_Rect square;
    Scene* scene;

public:
    Button(const string &fileName, Scene* owner_);
    ~Button();
    bool OnCreate();
    void Render();
    Vec3 buttonLocation(float x_, float y_, float z_);

};

#endif