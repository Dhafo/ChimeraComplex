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

   
    inverseProjection = MMath::inverse(projectionMatrix); // This must be added for method getMousePosition()


    /// Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);

    colorFont = { 255, 255, 255 };
    font = TTF_OpenFont("Lato-Regular.ttf", 24);

    // Buttons
    ReplayButton = new Button("ButtonBorder.png", Vec3(10.0, 8.0f, 0.0f), this);
    if (!ReplayButton->OnCreate()) {
        return false;
    }
    QuitButton = new Button("ButtonBorder.png", Vec3(10.0f, 6.0f, 0.0f), this);
    if (!QuitButton->OnCreate()) {
        return false;
    }

    // TTF texts
    restartText = TTF_RenderText_Solid(font, "Restart", colorFont);
    restartTexture = SDL_CreateTextureFromSurface(renderer, restartText);

    quitText = TTF_RenderText_Solid(font, "Quit", colorFont);
    quitTexture = SDL_CreateTextureFromSurface(renderer, quitText);

    endText = TTF_RenderText_Solid(font, "You Win!", colorFont);
    endTextTexture = SDL_CreateTextureFromSurface(renderer, endText);

    endCredits = TTF_RenderText_Solid(font, "Created by: Yev, Ramy, Peter, Ayo ", colorFont);
    endCreditsTexture = SDL_CreateTextureFromSurface(renderer, endCredits);
    
    // Rects
    endTextRect = { 430, 250,endText->w,endText->h };
    endCreditsRect = { 0, 600, endCredits->w, endCredits->h };

    restartTextRect = { 450, 300, restartText->w,restartText->h };
    quitTextRect = { 450, 385,quitText->w,quitText->h };

    background = { 0, 0, 960,640 };

    // Background image
    backgroundImage = IMG_Load("space.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);

   

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
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &background);
    ReplayButton->Render();
    QuitButton->Render();

   

    SDL_RenderCopy(renderer, endTextTexture, NULL, &endTextRect);
    SDL_RenderCopy(renderer, endCreditsTexture, NULL, &endCreditsRect);

    SDL_RenderCopy(renderer, restartTexture, NULL, &restartTextRect);

    SDL_RenderCopy(renderer, quitTexture, NULL, &quitTextRect);
  

    SDL_RenderPresent(renderer);
}

void Scene4::HandleEvents(const SDL_Event& event)
{
    Vec3 mousePos = getMousePosition();
    if (event.button.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
        if (ReplayButton->clicked(mousePos))
        {
            std::cout << " Replay Button clicked\n" << std::endl;
            // This code works when there is only have one button when moving scenes
            // it will not work when the buttonEnd exists
            //game->LoadScene(1);
            //This code works for changing scenes when the buttonEnd exists
            SDL_Event event;
            SDL_memset(&event, 0, sizeof(event));
            event.type = game->getChangeScence();
            event.user.code = 4; // moves back to scene0 (main menu)
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }

        if (QuitButton->clicked(mousePos))
        {
            std::cout << "Quit Button clicked\n" << std::endl;
            SDL_Event event;
            SDL_memset(&event, 0, sizeof(event));
            event.type = game->getChangeScence();
            event.user.code = 2; // ends game
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
    }
}

Vec3 Scene4::getMousePosition()
{
    Uint32 buttons;
    // mouse position in screen coords x,y
    int x, y;
    buttons = SDL_GetMouseState(&x, &y);
    Vec3 mouseScreenCoords = Vec3(float(x), float(y), 0.0f);
    Vec3 mouseWorldCoords = inverseProjection * (mouseScreenCoords);
    return mouseWorldCoords;
}