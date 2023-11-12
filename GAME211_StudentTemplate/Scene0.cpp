#include "Scene0.h"
#include <VMath.h>

// See notes about this constructor in Scene0.h.
Scene0::Scene0(SDL_Window* sdlWindow_, GameManager* game_) {
    window = sdlWindow_;
    game = game_;
    renderer = SDL_GetRenderer(window);
    xAxis = 25.0f;
    yAxis = 15.0f;
}

Scene0::~Scene0() {
}

bool Scene0::OnCreate() {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_RenderSetScale(renderer, 1, 1);
    Matrix4 ndc = MMath::viewportNDC(w, h);
    Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
    projectionMatrix = ndc * ortho;
    inverseProjection = MMath::inverse(projectionMatrix);

    /// Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);


    // Buttons
    buttonStart = new Button("ButtonBorder.png",Vec3(17.0,3.67f,0.0f), this);
    if (!buttonStart->OnCreate()) {
        return false;
    }
    buttonEnd = new Button("ButtonBorder.png", Vec3(17.0f, 2.55f, 0.0f), this);
    if (!buttonEnd->OnCreate()) {
        return false;
    }

    backgroundImage = IMG_Load("space.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);

    background = { 0, 0, 960,640 };

    TTF_Font* font;

    font = TTF_OpenFont("Lato-Regular.ttf", 24);

    if (!font) {
        std::cout << "Failed to load" << TTF_GetError() << std::endl;
    }
    else {
        // Set up a surface image with some text
        SDL_Surface* text;

        // Set color to white
        SDL_Color color = { 255,255,255 };

        text = TTF_RenderText_Solid(font, "Press X to Start", color);
        if (!text) {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else {
            //Set up a texture from the surface and render it
            text_texture = SDL_CreateTextureFromSurface(renderer, text);
            dest = { 512 + 128 + 32,256 + 128 - 16 + 112,text->w,text->h };
        }

        text = TTF_RenderText_Solid(font, "Press Q to Quit", color);
        if (!text) {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else {
            //Set up a texture from the surface and render it
            text_texture2 = SDL_CreateTextureFromSurface(renderer, text);
            dest2 = { 512 + 128 + 32,256 + 128 + 32 + 112,text->w,text->h };
        }
    }

    font = TTF_OpenFont("Lato-Regular.ttf", 80);
    if (!font) {
        std::cout << "Failed to load" << TTF_GetError() << std::endl;
    }
    else
    {
        SDL_Surface* text;
        // Set color to white
        SDL_Color color = { 242,140,40 };

        text = TTF_RenderText_Solid(font, "Chimera Complex", color);
        if (!text) {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else {
            //Set up a texture from the surface and render it
            text_texture3 = SDL_CreateTextureFromSurface(renderer, text);
            dest3 = { 256 - 64,256 - 80,text->w,text->h };
        }

    }

    game->getSoundEngine()->play2D("MyVeryOwnDeadShip.ogg", true);

    return true;
}

void Scene0::OnDestroy()
{
    SDL_DestroyTexture(text_texture);
}

void Scene0::Update(const float deltaTime) {

    // Update player
    game->getPlayer()->Update(deltaTime);

}

void Scene0::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgroundTexture, NULL, &background);

    // renders the button on the screen
    buttonStart->Render();
    buttonEnd->Render();

    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_RenderCopy(renderer, text_texture2, NULL, &dest2);
    SDL_RenderCopy(renderer, text_texture3, NULL, &dest3);

 

    SDL_RenderPresent(renderer);
}


void Scene0::HandleEvents(const SDL_Event& event)
{
    // send events to player as needed
    game->getPlayer()->HandleEvents(event);

    if (event.type == SDL_KEYDOWN) {
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


    // sends events when clicking a button
    Vec3 mousePos = getMousePosition();
    if (event.button.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {

        if (buttonStart->clicked(mousePos))
        {
            std::cout << "Start Button clicked\n" << std::endl;
            // This code works when there is only have one button when moving scenes
            // it will not work when the buttonEnd exists
            //game->LoadScene(1);

            //This code works for changing scenes when the buttonEnd exists
            SDL_Event event;
            SDL_memset(&event, 0, sizeof(event));
            event.type = game->getChangeScence();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);

        }

        if (buttonEnd->clicked(mousePos))
        {
            std::cout << "End Button clicked\n" << std::endl;
            void SDL_Quit(void); // For some reason this code runs but does not close the game
        }


    }


}


Vec3 Scene0::getMousePosition()
{
    Uint32 buttons;
    // mouse position in screen coords x,y
    int x, y; 
    buttons = SDL_GetMouseState(&x, &y);
    Vec3 mouseScreenCoords = Vec3(float(x), float(y), 0.0f);
    Vec3 mouseWorldCoords = inverseProjection * (mouseScreenCoords);
    return mouseWorldCoords;

}
