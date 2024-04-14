#ifndef SCENE3_H
#define Scene33H
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

class Scene3 : public Scene
{
private:
    SDL_GameController* gameController;
    float xAxis;     // scene width, in game coords, set in constructor
    float yAxis;	// scene height, in game coords, set in constructor
    SDL_Window* window;		// an SDL window with a SDL renderer
    SDL_Renderer* renderer;	// the renderer associated with SDL window
    Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4 inverseProjection;	// set in OnCreate()

    // to be used for tendril spawns
    float timeSecond;
    int time;

    float ammoTimer, healthTimer, skulker1Timer, skulker2Timer; // For the spawning of items/enemies

 
    Player player;
    //Arrays for each enemy type
    std::vector<Enemy*> skulker;


    std::vector<Enemy*> boss;
    
    std::vector<Enemy*> forwardTendrils;
    std::vector<Enemy*> sideTendrils;
    //Arrays for each item
    std::vector<Entity*> ammo;
    std::vector<Entity*> health;
    //Combined array holding each entity
    std::vector<Entity*> entities;
    float skulkADelay[2];
    int centerAttackPhase;

    int screenOffsetX = 16;
    int screenOffsetY = 16;

    

public:
    // This constructor may be different from what you've seen before
    // Notice the second parameter, and look in GameManager.cpp
    // to see how this constructor is called.
    Scene3(SDL_Window* sdlWindow, GameManager* game_);
    ~Scene3();

    bool OnCreate();
    void OnDestroy();

    void Update(const float time);
    void Render();
    void HandleEvents(const SDL_Event& event);
    void Shoot();
    void Interact();

    float getxAxis()
    {
        return xAxis;
    }
    float getyAxis()
    {
        return yAxis;
    }
    SDL_Window* getWindow()
    {
        return window;
    }
    Matrix4 getProjectionMatrix()
    {
        return projectionMatrix;
    }
    Matrix4 getInverseMatrix()
    {
        return inverseProjection;
    }

    int zBuffer[480]; //array that holds the depth at each ray hit

    //map wall information
    int mapWallsX = 16, mapWallsY = 16, mapWallsSize = 64;  // 16x16 map, each wall is a 64x64x64 "cube"
    int mapWalls[256] =
    {
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
       1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
       1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
       1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
       1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
       1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    //map ceiling information
    int mapCeilingX = 16, mapCeilingY = 16, mapCeilingSize = 64; // 16x16 map, each ceiling tile is a 64x64 square
    int mapCeiling[256] =
    {
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0,
      0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0,
      0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0,
      0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0,
      0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0,
      0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
      1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0,
      1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0,
      0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0,
      1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
      1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0,
    };

    //map floor information
    int mapFloorX = 16, mapFloorY = 16, mapFloorSize = 64; // 16x16 map, each ceiling tile is a 64x64 square
    int mapFloor[256] =
    {
      0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1,
      0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
      0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0,
      0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1,
      0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0,
      0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0,
      0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    //--

    //holds color values that are useful when drawing the walls and adding shading
    Vec3 color = Vec3(240, 240, 240);
    //renders a 2d map of the level on the screen, currently unused
    void drawMap2D();
    //renders the walls of the level in a "3D" projection on the screen, using raycasting
    void draw3D();
    //adds the floor and the ceiling into a pixel array that can later be rendered on the screen
    void drawFloorCeiling();
    //draws sprite of the entity provided (if it isn't getting covered by a wall)
    void entityTick(Entity* entity, SDL_Texture* entityTexture);
    //handles movement
    void HandleMovement();
    //sorts the array of entities in a way in which the last part is the closest to the player
    bool sortByDistance(Entity* entity1, Entity* entity2);
    //distance between two points
    float dist(float ax, float ay, float bx, float by);
    //returns true if enemy is not blocked by a wall and can see the player
    bool EnemyCanSeePlayer(Enemy* enemy_);
    //given an image, returns the color at the x,y pixel location
    Uint32 getpixel(SDL_Surface* surface, int x, int y);
    //every 3 seconds tendrals will spawn in a preset pattern
    void TendrilTimeing(int time);

    //handles the spawning of entities
    void Spawns(float deltatime);

    bool TendrilBlock(Enemy* enemy);

    //imported textures for level generation
    SDL_Surface* imageWall;
    SDL_Surface* imageWall2;
    SDL_Surface* imageDoor;
    SDL_Surface* imageDoor2;
    SDL_Surface* imageFloor;
    SDL_Surface* imageFloor2;
    SDL_Surface* imageCeiling;
    SDL_Surface* imageCeiling2;

    SDL_Texture* textureWall;
    SDL_Texture* textureWall2;
    SDL_Texture* textureDoor;
    SDL_Texture* textureDoor2;
    SDL_Texture* textureFloor;
    SDL_Texture* textureFloor2;
    SDL_Texture* textureCeiling;
    SDL_Texture* textureCeiling2;

    //imported textures for items
    SDL_Surface* keySprite;
    SDL_Surface* ammoSprite;
    SDL_Surface* healthSprite;

    SDL_Texture* keyTexture;
    SDL_Texture* ammoTexture;
    SDL_Texture* healthTexture;

    //imported textures for enemies

    SDL_Surface* skulkerSprite;
    SDL_Surface* bossSprite;
    SDL_Surface* tendrilSprite;
    SDL_Texture* skulkerTexture;
    SDL_Texture* bossTexture;
    SDL_Texture* tendrilTexture;

    // TTF stuff for the UI
    TTF_Font* font;
    SDL_Color colorFont;

    SDL_Surface* healthText;
    SDL_Texture* healthTextTexture;
    SDL_Surface* healthName;
    SDL_Texture* healthNameTexture;

    SDL_Surface* ammoText;
    SDL_Texture* ammoTextTexture;
    SDL_Surface* ammoName;
    SDL_Texture* ammoNameTexture;

    // SDL_Rect for UI
    SDL_Rect healthRect;
    SDL_Rect healthNameRect;
    SDL_Rect ammoRect;
    SDL_Rect ammoNameRect;

    SDL_Rect Top = { 0, 0, 512, screenOffsetY + 2 };
    SDL_Rect Bottom = { 0, 320 + screenOffsetY, 512, 64 };
    SDL_Rect Left = { 0, 0, screenOffsetX, 384 };
    SDL_Rect Right = { 496, 0, screenOffsetX, 384 };

    //for int to char* conversion of values
    char playerHealth[12];
    char playerAmmo[12];

    //gun anim
    SDL_Surface* imageGun;
    SDL_Texture* textureGun[6]; //6 frames
    int currentGunFrame = 0;
    float timePassedGun = 0.0f;
    bool shootGun = false;
    SDL_Rect gun = { screenOffsetX + 128,screenOffsetY + 128 + 16, 256, 256 };

    //player hit anim
    bool hit = false;
    float timePassedHit = 0.0f;
    int fade = 0;
    int fadeDir = 1;
    SDL_Rect dmgFade = { 0,0, 960, 640 };

    //player aimed successfully
    bool aim = false;
    float timePassedAim = 0.0f;

    float timePassedStep = 0.0f;
    SDL_Rect gameScreen = { screenOffsetX,screenOffsetY, 480, 320 };
    SDL_Rect background = { 0,0, 480, 420 };

    //UI key collection
    SDL_Rect keyAcq = { 224 + screenOffsetX, 326 + screenOffsetY, 32, 32 };

    //to hold our floor/ceiling rendering
    SDL_Texture* buffer = NULL;
    SDL_Surface* screenSurface = NULL;
    Uint32* pixels;
    int pitch;
};

#endif
