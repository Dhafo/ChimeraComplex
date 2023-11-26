#include "Scene4.h"
#include "VMath.h"

Scene4::Scene4(SDL_Window* sdlWindow_, GameManager* game_)
{
    window = sdlWindow_;
    game = game_;
    renderer = SDL_GetRenderer(window);
    xAxis = 25.0f;
    yAxis = 15.0f;
}

Scene4::~Scene4()
{
}

bool Scene4::OnCreate() {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_RenderSetScale(renderer, 1, 1);
    Matrix4 ndc = MMath::viewportNDC(w, h);
    Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
    projectionMatrix = ndc * ortho;

    /// Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);


    return true;
}

void Scene4::OnDestroy()
{
   

}

void Scene4::Update(const float deltaTime)
{
}


void Scene4::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    

    SDL_RenderPresent(renderer);
}

void Scene4::HandleEvents(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
       

    }
}