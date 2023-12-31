#ifndef Scene0_H
#define Scene0_H

#include <MMath.h>
#include "Scene.h"
#include "Button.h"

using namespace MATH;
class Scene0 : public Scene {
private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4 inverseProjection;	// set in OnCreate()

public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	Scene0(SDL_Window* sdlWindow, GameManager* game_);
	~Scene0();
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

    SDL_Surface* startImage;
    SDL_Texture* startTexture;

    SDL_Surface* endImage;
    SDL_Texture* endTexture;

    SDL_Surface* backgroundImage;
    SDL_Texture* backgroundTexture;

    Button *buttonStart;
    Button *buttonEnd;

    SDL_Texture* text_texture;
    SDL_Texture* text_texture2;
    SDL_Texture* text_texture3;
    SDL_Texture* text_lore;
    SDL_Rect dest;
    SDL_Rect dest2;
    SDL_Rect dest3;
    SDL_Rect dest4;
    SDL_Rect background;
    bool playing = true;

    Vec3 getMousePosition();
};

#endif
