#include "Scene1.h"
#include <VMath.h>

typedef struct
{
    int w, a, d, s;
}ButtonKeys; ButtonKeys Keys;

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
    imageWall = IMG_Load("wall.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
    textureWall = SDL_CreateTextureFromSurface(renderer, imageWall);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
    HandleMovement();
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
	SDL_RenderClear(renderer); 
    SDL_Rect ceiling = { 530, 16, 480, 160 };
    SDL_SetRenderDrawColor(renderer, 208, 255, 240, 0);
    SDL_RenderFillRect(renderer, &ceiling);
    SDL_Rect floor = { 530, 160, 480, 192 };
    SDL_SetRenderDrawColor(renderer, 68, 69, 45, 0);
    SDL_RenderFillRect(renderer, &floor);
    drawMap2D();
    draw3D();
    SDL_Rect Top = { 530, 0, 480, 16 };
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
    SDL_RenderFillRect(renderer, &Top);
    SDL_Rect Bottom = { 530, 320, 480, 192 };
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
    SDL_RenderFillRect(renderer, &Bottom);
	// render the player
	game->RenderPlayer(1.0f);

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{

        if (event.type == SDL_KEYDOWN)
        {
            
            if (event.key.keysym.scancode == SDL_SCANCODE_A) 
            {
                Keys.a = 1; 
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_D) 
            {
                Keys.d = 1;
            }

            if (event.key.keysym.scancode == SDL_SCANCODE_W) //fix later
            {
                Keys.w = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_S) //fix later
            {
                Keys.s = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_E)
            {
                int xo = 0, yo = 0;
                if (game->getPlayer()->vel.x < 0)
                {
                    xo = -25;
                }
                else
                {
                    xo = 25;
                }
                if (game->getPlayer()->vel.y < 0)
                {
                    yo = -25;
                }
                else
                {
                    yo = 25;
                }
                int ipx = game->getPlayer()->pos.x / 64.0, ipx_add_xo = (game->getPlayer()->pos.x + xo) / 64.0;
                int ipy = game->getPlayer()->pos.y / 64.0, ipy_add_yo = (game->getPlayer()->pos.y + yo) / 64.0;

                if (mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] == 4) 
                {
                    mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] = 0;
                }

            }
        }
        if(event.type == SDL_KEYUP)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_A)
            {
                Keys.a = 0;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_D)
            {
                Keys.d = 0;
            }

            if (event.key.keysym.scancode == SDL_SCANCODE_W) //fix later
            {
                Keys.w = 0;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_S) //fix later
            {
                Keys.s = 0;
            }
        }
}
        // send events to player as needed
       //game->getPlayer()->HandleEvents(event);

void Scene1::HandleMovement()
{
        if (Keys.a == 1)
        {
            game->getPlayer()->orientation -= 0.030;
            if (game->getPlayer()->orientation < 0)
            {
                game->getPlayer()->orientation += 2 * PI;
            }
            game->getPlayer()->vel.x = cos(game->getPlayer()->orientation) * 1.25f;
            game->getPlayer()->vel.y = sin(game->getPlayer()->orientation) * 1.25f;
        }
        if (Keys.d == 1)
        {
            game->getPlayer()->orientation += 0.030;
            if (game->getPlayer()->orientation > 2 * PI)
            {
                game->getPlayer()->orientation -= 2 * PI;
            }
            game->getPlayer()->vel.x = cos(game->getPlayer()->orientation) * 1.25f;
            game->getPlayer()->vel.y = sin(game->getPlayer()->orientation) * 1.25f;
        }

        int xo = 0, yo = 0;
        if (game->getPlayer()->vel.x < 0)
        {
            xo = -20;
        }
        else
        {
            xo = 20;
        }
        if (game->getPlayer()->vel.y < 0)
        {
            yo = -20;
        }
        else
        {
            yo = 20;
        }
        int ipx = game->getPlayer()->pos.x / 64.0, ipx_add_xo = (game->getPlayer()->pos.x + xo) / 64.0, ipx_sub_xo = (game->getPlayer()->pos.x - xo) / 64.0;
        int ipy = game->getPlayer()->pos.y / 64.0, ipy_add_yo = (game->getPlayer()->pos.y + yo) / 64.0, ipy_sub_yo = (game->getPlayer()->pos.y - yo) / 64.0;
        if (Keys.w == 1) //fix later
        {
            if (mapWalls[ipy * mapWallsX + ipx_add_xo] == 0)
            {
                game->getPlayer()->pos.x += game->getPlayer()->vel.x;

            }
            if (mapWalls[ipy_add_yo * mapWallsX + ipx] == 0)
            {
                game->getPlayer()->pos.y += game->getPlayer()->vel.y;
            }
        }
        if (Keys.s == 1) //fix later
        {
            if (mapWalls[ipy * mapWallsX + ipx_sub_xo] == 0)
            {
                game->getPlayer()->pos.x -= game->getPlayer()->vel.x;

            }
            if (mapWalls[ipy_sub_yo * mapWallsX + ipx] == 0)
            {
                game->getPlayer()->pos.y -= game->getPlayer()->vel.y;
            }
        }
}

void Scene1::drawMap2D()
{
    int x, y, xo, yo;
    for (y = 0; y < mapWallsY; y++)
    {
        for (x = 0; x < mapWallsX; x++)
        {
            if (mapWalls[y * mapWallsX + x] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 240, 240, 240, 0);

            }
            else if (mapWalls[y * mapWallsX + x] == 2)
            {
                SDL_SetRenderDrawColor(renderer, 240, 15, 15, 0);
            }
            else if (mapWalls[y * mapWallsX + x] == 4)
            {
                SDL_SetRenderDrawColor(renderer, 15, 15, 240, 0);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
            }
            SDL_Rect rect = { ((int)mapWallsS * x), ((int)mapWallsS * y), mapWallsS, mapWallsS };
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

    float rx, ry, ra;
    float xo = 0;
    float yo = 0;

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

    bool lastV = false;
    for(r=0; r<120; r++)
    {
        disT = 10000000;
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
            mp = my * mapWallsX + mx;
            if(mp > 0 && mp<mapWallsX * mapWallsY && mapWalls[mp] > 0)
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
            mp = my * mapWallsX + mx;
            if(mp > 0 && mp<mapWallsX * mapWallsY && mapWalls[mp] > 0)
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
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapWallsX + mx;
            if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 1)
            {
                color = Vec3(240, 240, 240);
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 2)
            {
                color = Vec3(240, 15, 15);
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 4)
            {
                color = Vec3(15, 15, 240);
            }
            SDL_SetRenderDrawColor(renderer,color.x, color.y, color.z, 0);
            SDL_SetTextureColorMod(textureWall, color.x, color.y, color.z);
        }
        else if (disH<disV)
        {
            rx = hX;
            ry = hY;
            disT = disH;
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapWallsX + mx;
            if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 1)
            {
                color = Vec3(240, 240, 240);
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 2)
            {
                color = Vec3(240, 15, 15);
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 4)
            {
                color = Vec3(15, 15, 240);
            }
            SDL_SetRenderDrawColor(renderer, color.x/1.6, color.y / 1.6, color.z / 1.6, 0);
            SDL_SetTextureColorMod(textureWall, color.x/1.6, color.y/1.6, color.z/1.6);
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

        disT *= cos(ca); //fisheye

        float lineH = (mapWallsS * 320) / disT;
        float lineO = 160 - lineH / 2;

        float texOffSet = 0;

        
        if (lineH > 320)
        {
            texOffSet = (lineH - 320) / 2.0;
            texOffSet = texOffSet * 64.0 / (float)lineH;
        }
        

        SDL_Rect rect = { r * 4 + 530, lineO + 16, 4, lineH + 16 };

        if (lineO < 0)
        {
            lineO = 0;
        }

        if (lineH > 320)
        {
            lineH = 320;
        }

        //where textures go
        int texVX = (ry / 64 - my) * (64);
        int texHX = (rx / 64 - mx) * (64);
        if (disV < disH) //check if vertical or horizontal
        {
            SDL_Rect crop = { texVX, 0, 1, 64}; // if vertical we use y offset
            SDL_RenderCopy(renderer, textureWall, &crop, &rect);        
        }
        else
        {
            SDL_Rect crop = { texHX, 0, 1, 64}; // if horizontal we use x offset
            SDL_RenderCopy(renderer, textureWall, &crop, &rect);
        }
        
        //SDL_RenderFillRect(renderer, &rect);
        //SDL_RenderDrawLine(renderer, r*8 + 530, lineO, r*8 + 530, lineH + lineO);
        //SDL_SetRenderDrawColor(renderer, 255, 15, 15, 0);
        //SDL_Rect TEST = { 128, 448, 64, 8 }; // if vertical we use y offset
        //SDL_RenderFillRect(renderer, &TEST);
       

        ra += DegToRad/2;
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
