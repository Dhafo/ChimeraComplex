#include "Button.h"

Button::Button(const string &fileName, Scene *owner_) {
    
    scene = owner_;

    image = IMG_Load(fileName.c_str());
    
}

Button::~Button()
{}

bool Button::OnCreate()
{
    Vec3 screenCoords;
    int    w, h;
    SDL_Renderer* renderer;
    SDL_Window* window = scene->getWindow();
    renderer = SDL_GetRenderer(window);

    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (image == nullptr) {
        std::cerr << "Can't open the image" << std::endl;
    }

    Vec3 pos = Vec3(14.0f, 10.0f, 0.0f);
    float scale = 0.15f;

    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    // convert the position from game coords to screen coords.
    screenCoords = scene->getProjectionMatrix() * pos;

    // Scale the image, in case the .png file is too big or small

    w = image->w * scale;
    h = image->h * scale;

    // The square's x and y values represent the top left corner of 
    // where SDL will draw the .png image.
    // The 0.5f * w/h offset is to place the .png so that pos represents the center
    // (Note the y axis for screen coords points downward, hence subtraction!!!!)
    square.x = static_cast<int>(screenCoords.x - 0.5f * w);
    square.y = static_cast<int>(screenCoords.y - 0.5f * h);
    square.w = static_cast<int>(w);
    square.h = static_cast<int>(h);

    return true;
}


void Button::Render()
{

 

    // SDL_Renderer* renderer;
    // SDL_Window* window = scene->getWindow();
    // renderer = SDL_GetRenderer(window);


    ////

    ////// Convert character orientation from radians to degrees.
    //float orientationDegrees = 0.0f; // orientation * 180.0f / M_PI;

    //SDL_RenderCopyEx(renderer, texture, nullptr, &square,
    //    orientationDegrees, nullptr, SDL_FLIP_NONE);


}

Vec3 Button::buttonLocation(float x_, float y_, float z_)
{
    Vec3 screenCoords;
    int    w, h;
    SDL_Renderer* renderer;
    SDL_Window* window = scene->getWindow();
    renderer = SDL_GetRenderer(window);

    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (image == nullptr) {
        std::cerr << "Can't open the image" << std::endl;
    }
    Vec3 pos = Vec3(x_, y_, z_);
    float scale = 0.15f;

    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    // convert the position from game coords to screen coords.
    screenCoords = scene->getProjectionMatrix() * pos;

    // Scale the image, in case the .png file is too big or small

    w = image->w * scale;
    h = image->h * scale;

    // The square's x and y values represent the top left corner of 
    // where SDL will draw the .png image.
    // The 0.5f * w/h offset is to place the .png so that pos represents the center
    // (Note the y axis for screen coords points downward, hence subtraction!!!!)
    square.x = static_cast<int>(screenCoords.x - 0.5f * w);
    square.y = static_cast<int>(screenCoords.y - 0.5f * h);
    square.w = static_cast<int>(w);
    square.h = static_cast<int>(h);

    return Vec3();
}
