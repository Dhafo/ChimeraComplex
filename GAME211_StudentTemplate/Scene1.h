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

    //map
    int mapX = 8, mapY = 8, mapS = 64;
    int map[64] =
    {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 2, 2, 0, 1, 0, 1,
        1, 0, 2, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
    };

    Vec3 color = Vec3(240, 240, 240);

    void drawMap2D();
    void draw3D();
    void HandleMovement();
    int FixAng(int a) { if (a > 359) { a -= 360; } if (a < 0) { a += 360; } return a; }

    SDL_Surface* imageWall;
    SDL_Texture* textureWall;

<<<<<<< Updated upstream
    Uint32 buffer[512][1024];
=======
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


    // TTF stuff for the UI
    TTF_Font* font;
    SDL_Color colorFont;

    SDL_Surface* health;
    SDL_Texture* healthTexture;
    SDL_Surface* healthName;
    SDL_Texture* healthNameTexture;

    SDL_Surface* ammo;
    SDL_Texture* ammoTexture;
    SDL_Surface* ammoName;
    SDL_Texture* ammoNameTexture;

    SDL_Surface* keyName;
    SDL_Texture* keyNameTexture;

    // png for the key in the UI
    SDL_Surface* cardKey;
    SDL_Texture* cardKeyTexture;

    // SDL_Rect for UI
    SDL_Rect healthRect;
    SDL_Rect healthNameRect;
    SDL_Rect ammoRect;
    SDL_Rect ammoNameRect;
    SDL_Rect keyNameRect;
    SDL_Rect cardKeyRect;

    //For the _itoa_
    char playerHealth[12];
    char playerAmmo[12];



    int currentGunFrame = 0;
    bool shootGun = false;
    bool hit = false;
    float timePassedGun = 0.0f;
    float timePassedHit = 0.0f;
    int fade = 0;
    int fadeDir = 1;
    SDL_Rect gun = { 128, 128 - 8, 256, 256 };
    //UI key collection
    SDL_Rect keyAcq = { 32, 290, 64, 64 };
    


    SDL_Texture* buffer = NULL;
    SDL_Surface* surf = NULL;

    Uint32* pixels;
    int pitch;
>>>>>>> Stashed changes
};

#endif
