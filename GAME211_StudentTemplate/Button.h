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

    // stuff needed to detect a click
    Vec3 position;
    Vec3 topLeft;
    Vec3 bottomRight;

public:
    Button(const string& fileName, Vec3 position, Scene* owner_);
    ~Button();
    bool OnCreate();
    void OnDestroy();
    void Render();
    bool clicked(Vec3 mousePos);

};

#endif