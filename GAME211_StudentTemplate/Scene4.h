#ifndef Scene4_H
#define Scene4_H

#include <MMath.h>
#include "Scene.h"
#include "Button.h"

using namespace MATH;
class Scene4 : public Scene {
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
    Scene4(SDL_Window* sdlWindow, GameManager* game_);
    ~Scene4();
    bool OnCreate();
    void OnDestroy();
    void Update(const float time);
    void Render();
    void HandleEvents(const SDL_Event& event);
    float getxAxis() { return xAxis; }
    float getyAxis() { return yAxis; }
    SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
    Matrix4 getInverseMatrix() { return inverseProjection; }

    TTF_Font* font;
    SDL_Color colorFont;

    //button
    Button* ReplayButton;
    Button* QuitButton;
    Vec3 getMousePosition();

    // TTF texts
    SDL_Surface* endText;
    SDL_Texture* endTextTexture;

    SDL_Surface* endCredits;
    SDL_Texture* endCreditsTexture;

    SDL_Surface* restartText;
    SDL_Texture* restartTexture;

    SDL_Surface* quitText;
    SDL_Texture* quitTexture;


    // Rect
    SDL_Rect endTextRect;
    SDL_Rect endCreditsRect;
    SDL_Rect restartTextRect;
    SDL_Rect quitTextRect;
    SDL_Rect background;


    // Background
    SDL_Surface* backgroundImage;
    SDL_Texture* backgroundTexture;

    bool playing = true;

    

};
#endif

