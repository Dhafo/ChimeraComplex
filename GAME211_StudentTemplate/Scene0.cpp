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
    playing = true;
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_RenderSetScale(renderer, 1, 1);
    Matrix4 ndc = MMath::viewportNDC(w, h);
    Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
    projectionMatrix = ndc * ortho;
    inverseProjection = MMath::inverse(projectionMatrix);
    /// Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);

    // Set player image to PacMan

    // Buttons
    buttonStart = new Button("ButtonBorder.png", Vec3(15.8, 4.35f, 0.0f), this);
    if (!buttonStart->OnCreate()) {
        return false;
    }
    buttonEnd = new Button("ButtonBorder.png", Vec3(15.8f, 3.379f, 0.0f), this);
    if (!buttonEnd->OnCreate()) {
        return false;
    }

    backgroundImage = IMG_Load("space.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);

    background = { 0, 0, 1024,768 };

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

        text = TTF_RenderText_Solid(font, "Start New Game", color);
        if (!text) {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else {
            //Set up a texture from the surface and render it
            text_texture = SDL_CreateTextureFromSurface(renderer, text);
            dest = { 672,544,text->w,text->h };
        }
        SDL_FreeSurface(text);
        text = TTF_RenderText_Solid(font, "Quit", color);
        if (!text) {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else {
            //Set up a texture from the surface and render it
            text_texture2 = SDL_CreateTextureFromSurface(renderer, text);
            dest2 = { 672,592,text->w,text->h };
        }
        SDL_FreeSurface(text);
        text = TTF_RenderText_Solid_Wrapped(font, "The facility was once hailed as the pinnacle of scientific achievement, now it's teeming with grotesque monsters. You're just another mercenary that has been thrown at the problem. You have to get to the source.", color, 500);
        if (!text) {
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        }
        else {
            //Set up a texture from the surface and render it
            text_lore = SDL_CreateTextureFromSurface(renderer, text);
            dest4 = { 136,448,text->w,text->h };
        }
        SDL_FreeSurface(text);
    }
    TTF_CloseFont(font);
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
        SDL_FreeSurface(text);

    }

    game->getSoundEngine()->play2D("Audio/MyVeryOwnDeadShip.ogg", true);
    TTF_CloseFont(font);
    return true;
}

void Scene0::OnDestroy()
{
    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(text_texture2);
    SDL_DestroyTexture(text_texture3);
    SDL_DestroyTexture(text_lore);
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(backgroundImage);

    buttonStart->OnDestroy();
    buttonEnd->OnDestroy();

    delete buttonStart;
    delete buttonEnd;
}

void Scene0::Update(const float deltaTime) {

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
    SDL_RenderCopy(renderer, text_lore, NULL, &dest4);
    SDL_RenderPresent(renderer);
}


void Scene0::HandleEvents(const SDL_Event& event)
{
    // send events to player as needed
    if(playing)
    {
        Vec3 mousePos = getMousePosition();
        if (event.button.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            if (buttonStart != NULL && buttonStart->clicked(mousePos))
            {
                std::cout << "Start Button clicked\n" << std::endl;
                game->getSoundEngine()->play2D("Audio/beep.wav", false);
                playing = false;
                game->LoadScene(1);
            }

            else if (buttonEnd != NULL && buttonEnd->clicked(mousePos))
            {
                std::cout << "End Button clicked\n" << std::endl;
                game->Quit();
            }
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
