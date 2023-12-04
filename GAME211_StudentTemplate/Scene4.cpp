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
    playing = true;
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
    ReplayButton = new Button("ButtonBorder.png", Vec3(15.8, 4.35f, 0.0f), this);
    if (!ReplayButton->OnCreate()) {
        return false;
    }
    QuitButton = new Button("ButtonBorder.png", Vec3(15.8f, 3.379f, 0.0f), this);
    if (!QuitButton->OnCreate()) {
        return false;
    }

    // TTF texts
    restartText = TTF_RenderText_Solid(font, "Restart", colorFont);
    restartTexture = SDL_CreateTextureFromSurface(renderer, restartText);

    quitText = TTF_RenderText_Solid(font, "Quit", colorFont);
    quitTexture = SDL_CreateTextureFromSurface(renderer, quitText);

    endText = TTF_RenderText_Solid_Wrapped(font, "With unwavering determination, you emerged victorious, the facility now silent, save for your echoing footsteps.", colorFont, 600);
    endTextTexture = SDL_CreateTextureFromSurface(renderer, endText);

    endCredits = TTF_RenderText_Solid(font, "Created by: Yev, Ramy, Peter, Ayo (Attribution in ReadMe)", colorFont);
    endCreditsTexture = SDL_CreateTextureFromSurface(renderer, endCredits);
    
    // Rects
    endTextRect = { 128 + 64 + 8, 250 + 8,endText->w,endText->h };
    endCreditsRect = { 16, 732, endCredits->w, endCredits->h };

    restartTextRect = { 672,544, restartText->w,restartText->h };
    quitTextRect = { 672,592,quitText->w,quitText->h };

    background = { 0, 0, 1024,768 };

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
    if (playing)
    {
        Vec3 mousePos = getMousePosition();
        if (event.button.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            if (ReplayButton != NULL && ReplayButton->clicked(mousePos))
            {
                std::cout << "Start Button clicked\n" << std::endl;
                game->getSoundEngine()->play2D("Audio/beep.wav", false);
                playing = false;
                game->LoadScene(1);
            }

            else if (QuitButton != NULL && QuitButton->clicked(mousePos))
            {
                std::cout << "End Button clicked\n" << std::endl;
                game->Quit();
            }
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