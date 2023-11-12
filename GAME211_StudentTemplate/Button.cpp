#include "Button.h"

Button::Button(const string& fileName, Vec3 position_, Scene* owner_) {
    scene = owner_;
    image = IMG_Load(fileName.c_str());
    square = { 0,0,0,0 };
    texture = NULL;
    position = position_;
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

    // change this value to scale the image
    float scale = 1.0f;

    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    // convert the position from game coords to screen coords.
    screenCoords = scene->getProjectionMatrix() * position;

    // Scale the image, in case the .png file is too big or small

    w = image->w * scale;
    h = image->h * scale;

    // The square's x and y values represent the top left corner of 
    // where SDL will draw the .png image.
    // The 0.5f * w/h offset is to place the .png so that pos represents the center
    // (Note the y axis for screen coords points downward, hence subtraction!!!!)
    square.x = static_cast<int>(screenCoords.x);
    square.y = static_cast<int>(screenCoords.y);
    square.w = static_cast<int>(w);
    square.h = static_cast<int>(h);

    topLeft = position;
    bottomRight = scene->getInverseMatrix() * Vec3(static_cast<float>(square.x + w), static_cast<float>(square.y + h), 1.0f);


    return true;
}


void Button::Render()
{
     SDL_Renderer* renderer;
     SDL_Window* window = scene->getWindow();
     renderer = SDL_GetRenderer(window);

    //// Convert character orientation from radians to degrees.
    float orientationDegrees = 0.0f; // orientation * 180.0f / M_PI;

    SDL_RenderCopyEx(renderer, texture, nullptr, &square,
        orientationDegrees, nullptr, SDL_FLIP_NONE);

}

bool Button::clicked(Vec3 mousePos)
{
   // std::cout << "clicked" << std::endl;
    return(topLeft.x < mousePos.x && mousePos.x < bottomRight.x && bottomRight.y < mousePos.y && mousePos.y < topLeft.y);
}

