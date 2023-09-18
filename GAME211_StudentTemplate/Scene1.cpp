#include "Scene1.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 1024.0f;
	yAxis = 512.0f;
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
    draw3D();

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

float dist(float ax, float ay, float bx, float by, float ang)
{
    return(sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void Scene1::draw3D()
{
    int r, mx, my, mp, dof;

    float rx, ry, ra, xo, yo;

    float disT;

    PlayerBody* player = game->getPlayer();

    ra = player->orientation - DegToRad * 30;

    if(ra < 0)
    {
        ra += 2 * PI;
    }
    if(ra > 2*PI)
    {
        ra -= 2 * PI;
    }

    for(r=0; r<60; r++)
    {
        // Horizontal Line Check
        dof = 0;
        float disH = 100000000;
        float hX = player->pos.x;
        float hY = player->pos.y;
        float aTan = -1 / tan(ra);
        if(ra > PI)
        {
            ry = (int)(player->pos.y / 64) * 64 - 0.0001;
            rx = (player->pos.y - ry) * aTan + player->pos.x;
            yo = -64;
            xo = -yo * aTan;
        }
        if (ra < PI)
        {
            ry = (int)(player->pos.y / 64) * 64 + 64;
            rx = (player->pos.y - ry) * aTan + player->pos.x;
            yo = 64;
            xo = -yo * aTan;
        }
        if(ra ==0 || ra ==PI)
        {
            rx = player->pos.x;
            ry = player->pos.y;
            dof = 8;
        }
        while(dof < 8)
        {
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapX + mx;
            if(mp > 0 && mp<mapX * mapY && map[mp] == 1)
            {
                hX = rx;
                hY = ry;
                disH = dist(player->pos.x, player->pos.y, hX, hY, ra);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }    
        // Vertical Line Check
        dof = 0;
        float disV = 100000000;
        float vX = player->pos.x;
        float vY = player->pos.y;
        float nTan = -tan(ra);
        if(ra > P2 && ra < P3)
        {
            rx = (int)(player->pos.x / 64) * 64 - 0.0001;
            ry = (player->pos.x - rx) * nTan + player->pos.y;
            xo = -64;
            yo = -xo * nTan;
        }
        if (ra < P2 || ra > P3)
        {
            rx = (int)(player->pos.x / 64) * 64 + 64;
            ry = (player->pos.x - rx) * nTan + player->pos.y;
            xo = 64;
            yo = -xo * nTan;
        }
        if(ra ==0 || ra ==PI)
        {
            rx = player->pos.x;
            ry = player->pos.y;
            dof = 8;
        }
        while(dof < 8)
        {
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapX + mx;
            if(mp > 0 && mp<mapX * mapY && map[mp] == 1)
            {
                vX = rx;
                vY = ry;
                disV = dist(player->pos.x, player->pos.y, vX, vY, ra);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        if(disV<disH)
        {
            rx = vX;
            ry = vY;
            disT = disV;
            SDL_SetRenderDrawColor(renderer, 255, 15, 15, 0);
        }
        else if (disH<disV)
        {
            rx = hX;
            ry = hY;
            disT = disH;
            SDL_SetRenderDrawColor(renderer, 200, 15, 15, 0);
        }
        SDL_RenderDrawLine(renderer, player->pos.x, player->pos.y, rx, ry);

        //3D Wall Drawing
        float ca = player->orientation - ra;

        if (ca < 0)
        {
            ca += 2 * PI;
        }
        if (ca > 2 * PI)
        {
            ca -= 2 * PI;
        }

        disT = disT * cos(ca); //fisheye

        float lineH = (mapS * 320) / disT;
        float lineO = 160 - lineH / 2;
        if(lineH > 320)
        {
            lineH = 320;
        }
        SDL_RenderDrawLine(renderer, r*8 + 530, lineO, r*8 + 530, lineH + lineO);
        //

        ra += DegToRad;
        if (ra < 0)
        {
            ra += 2 * PI;
        }
        if (ra > 2 * PI)
        {
            ra -= 2 * PI;
        }
    }

}
