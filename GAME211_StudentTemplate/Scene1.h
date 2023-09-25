#ifndef SCENE1_H
#define SCENE1_H

#include <MMath.h>
#include <vector>
#include "Scene.h"

using namespace MATH;
class Scene1 : public Scene {
private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4     inverseProjection;	// set in OnCreate()

public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
    void HandleEvents(const SDL_Event &event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }

    //map Walls
    int mapWallsX = 8, mapWallsY = 8, mapWallsS = 64;
    int mapWalls[64] =
    {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 2, 2, 4, 1, 0, 1,
        1, 0, 2, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
    };

    //map Ceiling
    int mapCeilingX = 8, mapCeilingY = 8, mapCeilingS = 64;
    int mapCeiling[64] =
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 0, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

    //map Floor
    int mapFloorX = 8, mapFloorY = 8, mapFloorS = 64;
    int mapFloor[64] =
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 0, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

    Vec3 color = Vec3(240, 240, 240);

    void drawMap2D();
    void draw3D();
    void HandleMovement();
    int FixAng(int a) { if (a > 359) { a -= 360; } if (a < 0) { a += 360; } return a; }

    SDL_Surface* imageWall;
    SDL_Texture* textureWall;
};

#endif
