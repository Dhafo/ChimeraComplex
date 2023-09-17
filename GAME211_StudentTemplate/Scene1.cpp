#include "Scene1.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
}

Scene1::~Scene1(){
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to PacMan

	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer); 

    drawMap2D();

	// render the player
	game->RenderPlayer(1.0f);

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}

void Scene1::drawMap2D()
{
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++)
    {
        for (x = 0; x < mapX; x++)
        {
            if (map[y * mapX + x] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 240, 240, 240, 0);

            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
            }
            //xo = x * mapS;
            //yo = y * mapS;
            //glBegin(GL_QUADS);
            //glVertex2i(xo, yo);
            //glVertex2i(xo, yo+mapS);
            //glVertex2i(xo+mapS, yo+mapS):
            //glVertex2i(xo+mapS, yo);
            //glEnd();
            SDL_Rect rect = { ((int)mapS * x), ((int)mapS * y), mapS, mapS };
            SDL_RenderFillRect(renderer, &rect);

        }
    }
}
