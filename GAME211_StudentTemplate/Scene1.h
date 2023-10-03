#ifndef SCENE1_H
#define SCENE1_H
#include <iostream>
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
    bool kCollected;

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
    int mapWallsX = 16, mapWallsY = 16, mapWallsS = 64;
    int mapWalls[256] =
    {
      2, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2,
      2, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2,
      2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 2, 2, 0, 1,
      1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
      1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 2, 1,
      1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 2,
      1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1,
      1, 1, 2, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 2, 0, 1, 0, 1, 1,
      1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 1, 0, 0, 1,
      2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 1, 0, 1,
      2, 0, 0, 0, 0, 0, 2, 0, 4, 0, 1, 1, 0, 1, 0, 1,
      2, 0, 0, 0, 0, 0, 1, 1, 5, 1, 0, 0, 0, 1, 0, 1,
      2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
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
    void drawFloors();
    void entityTick();
    void HandleMovement();
    int FixAng(int a) { if (a > 359) { a -= 360; } if (a < 0) { a += 360; } return a; }

    void Key();
    SDL_Surface* imageWall;
    SDL_Texture* textureWall;
    SDL_Surface* enemySprite;
    SDL_Texture* enemyTexture;

    SDL_Rect Top = { 530, 0, 500, 16 };
    SDL_Rect Bottom = { 530, 320, 500, 192 };
    SDL_Rect Left = { 512, 0, 24, 512 };
    SDL_Rect Right = { 1010, 0, 32, 512 };

    SDL_Rect keyAcq = { 530 + 16, 320 + 32, 32, 32 };
    SDL_Rect keyMap = { 2 * 64, 2 * 64, 4, 4 };
};

#endif
