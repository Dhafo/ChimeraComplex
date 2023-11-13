#include "Scene0.h"
#include <VMath.h>

// See notes about this constructor in Scene0.h.
Scene0::Scene0(SDL_Window* sdlWindow_, GameManager* game_) 
{
    window = sdlWindow_;
    game = game_;
    renderer = SDL_GetRenderer(window);
    xAxis = 25.0f;
    yAxis = 15.0f;
}

Scene0::~Scene0()
{
}

bool Scene0::OnCreate()
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_RenderSetScale(renderer, 1, 1);
    Matrix4 ndc = MMath::viewportNDC(w, h);
    Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
    projectionMatrix = ndc * ortho;

    /// Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);

    startImage = IMG_Load("Start.png");
    startTexture = SDL_CreateTextureFromSurface(renderer, startImage);

    endImage = IMG_Load("End.png");
    endTexture = SDL_CreateTextureFromSurface(renderer, endImage);

    backgroundImage = IMG_Load("space.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);

    background = { 0, 0, 960,640 };

    TTF_Font* font;
    font = TTF_OpenFont("Lato-Regular.ttf", 24);
    if (!font) 
    {
        std::cout << "Failed to load" << TTF_GetError() << std::endl;
    }
    else 
    {
        // Set up a surface image with some text
        SDL_Surface* text;

        // Set color to white
        SDL_Color color = { 255,255,255 };

        text = TTF_RenderText_Solid(font, "Press X to Start", color);
        if (!text) 
        {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else 
        {
            //Set up a texture from the surface and render it
            text_texture = SDL_CreateTextureFromSurface(renderer, text);
            dest = { 672,480,text->w,text->h };
        }

        text = TTF_RenderText_Solid(font, "Press Q to Quit", color);
        if (!text)
        {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else 
        {
            //Set up a texture from the surface and render it
            text_texture2 = SDL_CreateTextureFromSurface(renderer, text);
            dest2 = { 672,528,text->w,text->h };
        }
    }

    font = TTF_OpenFont("Lato-Regular.ttf", 80);
    if (!font) 
    {
        std::cout << "Failed to load" << TTF_GetError() << std::endl;
    }
    else
    {
        SDL_Surface* text;
        // Set color to orange
        SDL_Color color = { 242,140,40 };
        text = TTF_RenderText_Solid(font, "Chimera Complex", color);
        if (!text) 
        {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else 
        {
            //Set up a texture from the surface and render it
            text_texture3 = SDL_CreateTextureFromSurface(renderer, text);
            dest3 = { 192,176,text->w,text->h };
        }

    }
    //play music
    game->getSoundEngine()->play2D("MyVeryOwnDeadShip.ogg", true);
    return true;
}

void Scene0::OnDestroy()
{
    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(text_texture2);
    SDL_DestroyTexture(text_texture3);

    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(endTexture);
    SDL_DestroyTexture(backgroundTexture);

    SDL_FreeSurface(startImage);
    SDL_FreeSurface(endImage);
    SDL_FreeSurface(backgroundImage);
}

void Scene0::Update(const float deltaTime)
{
}

void Scene0::Render() 
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgroundTexture, NULL, &background);

    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_RenderCopy(renderer, text_texture2, NULL, &dest2);
    SDL_RenderCopy(renderer, text_texture3, NULL, &dest3);

    SDL_RenderPresent(renderer);
}

void Scene0::HandleEvents(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN) 
    {
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_X:
            game->getSoundEngine()->play2D("beep.wav", false);
            game->LoadScene(1);
            break;

        case SDL_SCANCODE_Q:
            std::cout << "Q is pressed" << std::endl;
            void SDL_Quit(void);
            break;
        }

    }
}
