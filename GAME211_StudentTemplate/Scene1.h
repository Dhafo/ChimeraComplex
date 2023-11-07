#ifndef SCENE1_H
#define SCENE1_H
#include <iostream>
#include <MMath.h>
#include <vector>
#include "Scene.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

using namespace MATH;

//resources that helped me: 
// https://lodev.org/cgtutor/raycasting.html
// https://wynnliam.github.io/raycaster/news/tutorial/2019/04/03/raycaster-part-02.html
// https://www.youtube.com/watch?v=gYRrGTC7GtA
//

class Scene1 : public Scene {


private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4     inverseProjection;	// set in OnCreate()
    bool kCollected,aCollected,hCollected; //if key, ammo, health are collected
    Entity key,ammoItem,healthItem; 
    Enemy skulker1;
    Player player;
    std::vector<Enemy*> skulker;// Array for the Skulker Enemies

    std::vector<Enemy*> predator;// Array for the Skulker Enemies
    bool predCanSee[1];

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
    void EnemyMoveUpate(Enemy* enemy_);
    int zBuffer[480]; //depth at each ray hit

    //map Walls
    int mapWallsX = 16, mapWallsY = 16, mapWallsS = 64;
    int mapWalls[256] =
    {
      2, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2,
      2, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2,
      2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 2,
      1, 0, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
      1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 2, 2, 0, 1,
      1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
      1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 4, 1, 4, 2, 1,
      1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 2,
      1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1,
      1, 1, 2, 4, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 1, 1, 4, 1, 2, 0, 1, 0, 1, 1,
      1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 1, 0, 0, 1,
      2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 1, 0, 1,
      2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 0, 1, 0, 1,
      2, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1,
      2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 5, 1,
    };

    //placeholder --- 
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
    //--
  
    Vec3 color = Vec3(240, 240, 240);

    //draws map on left side of screen
    void drawMap2D();
    //draws 3d view
    void draw3D();
    //WIP
    void drawFloors();
    //draws sprite
    void entityTick();
    //handles movemnet
    void HandleMovement();
    int FixAng(int a) { if (a > 359) { a -= 360; } if (a < 0) { a += 360; } return a; }
    float dist(float ax, float ay, float bx, float by, float ang);
    //imported textures
    SDL_Surface* imageWall;
    SDL_Texture* textureWall;
    SDL_Surface* imageWall2;
    SDL_Texture* textureWall2;
    SDL_Surface* imageDoor;
    SDL_Texture* textureDoor;
    SDL_Surface* imageDoor2;
    SDL_Texture* textureDoor2;
    SDL_Surface* enemySprite;
    SDL_Texture* enemyTexture;

    //cover gameview with UI background
    SDL_Rect Top = { 530, 0, 500, 16 };
    SDL_Rect Bottom = { 530, 320, 500, 192 };
    SDL_Rect Left = { 512, 0, 24, 512 };
    SDL_Rect Right = { 1010, 0, 32, 512 };

    //UI key collection
    SDL_Rect keyAcq = { 530 + 32, 320 + 48, 64, 64 };
    //UI key on map
    SDL_Rect keyMap = { 192/2, 896/2, 4, 4 };
    
};

#endif
