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
    Player player;
    std::vector<Enemy*> skulker;// Array for the Skulker Enemies

    std::vector<Enemy*> predator;// Array for the Skulker Enemies
    std::vector<Entity*> entities;
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
    bool sortByDistance(Entity* entity1, Entity* entity2);
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
    int mapCeilingX = 16, mapCeilingY = 16, mapCeilingS = 64;
    int mapCeiling[256] =
    {
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
    };

    //map Floor
    int mapFloorX = 16, mapFloorY = 16, mapFloorS = 64;
    int mapFloor[256] =
    {
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
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
    void entityTick(Entity* entity, SDL_Texture* entityTexture);
    //handles movemnet
    void HandleMovement();
    int FixAng(int a) { if (a > 359) { a -= 360; } if (a < 0) { a += 360; } return a; }
    float dist(float ax, float ay, float bx, float by);
    //imported textures
    SDL_Surface* imageWall;
    SDL_Texture* textureWall;
    SDL_Surface* imageWall2;
    SDL_Texture* textureWall2;
    SDL_Surface* imageDoor;
    SDL_Texture* textureDoor;
    SDL_Surface* imageDoor2;
    SDL_Texture* textureDoor2;
    SDL_Surface* keySprite;
    SDL_Texture* keyTexture;

    SDL_Surface* predatorSprite;
    SDL_Texture* predatorTexture;

    SDL_Surface* skulkerSprite;
    SDL_Texture* skulkerTexture;

    SDL_Surface* imageFloor;
    SDL_Texture* textureFloor;

    SDL_Surface* imageCeiling;
    SDL_Texture* textureCeiling;

    SDL_Surface* imageGun;
    SDL_Texture* textureGun[6]; //6 frames
    int currentGunFrame = 0;
    bool shootGun = false;
    float timePassed = 0.0f;
    SDL_Rect gun = { 128, 128 - 8, 256, 256 };
    //UI key collection
    SDL_Rect keyAcq = { 32, 290, 64, 64 };
    


    SDL_Texture* buffer = NULL;
    SDL_Surface* surf = NULL;

    Uint32* pixels;
    int pitch;
};

#endif
