#include "Scene1.h"
#include <VMath.h>
#include <iostream>

using namespace std;
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
    kCollected = false;
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
    enemySprite = IMG_Load("enemyPlaceholder.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
    textureWall = SDL_CreateTextureFromSurface(renderer, imageWall);
    enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySprite);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	return true;
}

void Scene1::OnDestroy() 
{
    SDL_DestroyTexture(textureWall);
    SDL_FreeSurface(imageWall);
    SDL_DestroyTexture(enemyTexture);
    SDL_FreeSurface(enemySprite);
}

void Scene1::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
    HandleMovement();
    // 4/2 because of map size(4,4) = x,y
    if (!kCollected && ((2*64) + 2 >= game->getPlayer()->pos.x && game->getPlayer()->pos.x >= 2*64 - 2) && ((2 * 64) + 4 >= game->getPlayer()->pos.y && game->getPlayer()->pos.y >= 2 * 64 - 2))
    { 
        std::cout << "Blue Key Acquired!" << std::endl;    
        kCollected = true;
    }
}



void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
	SDL_RenderClear(renderer); 

    draw3D();
    entityTick();

    drawMap2D();
   
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
    //UI
    SDL_RenderFillRect(renderer, &Top);
    SDL_RenderFillRect(renderer, &Bottom);;
    SDL_RenderFillRect(renderer, &Left);
    SDL_RenderFillRect(renderer, &Right);
    
    if (kCollected)
    {
        // key collect UI
        SDL_SetRenderDrawColor(renderer, 15, 15, 255, 0);
        SDL_RenderFillRect(renderer, &keyAcq);
    }
    else
    {
        //key map dot
        SDL_SetRenderDrawColor(renderer, 15, 15, 255, 0);
        SDL_RenderFillRect(renderer, &keyMap);
    }

	// render the player
	//game->RenderPlayer(1.0f);
    SDL_Rect playerPos = { (game->getPlayer()->pos.x - 2)/2, (game->getPlayer()->pos.y - 2)/2, 4, 4 };   
    SDL_SetRenderDrawColor(renderer, 255, 255, 15, 0); 
    SDL_RenderFillRect(renderer, &playerPos);

    // render direction on map
    int pointX = (game->getPlayer()->pos.x) / 2;
    pointX += cos(-game->getPlayer()->orientation) * (((game->getPlayer()->pos.x) / 2 + 8) - (game->getPlayer()->pos.x) / 2) - sin(-game->getPlayer()->orientation) * (0);
    int pointY = (game->getPlayer()->pos.y) / 2;
    pointY += sin(-game->getPlayer()->orientation) * (((game->getPlayer()->pos.x) / 2 + 8) - (game->getPlayer()->pos.x) / 2) + cos(-game->getPlayer()->orientation) * (0);
    SDL_RenderDrawLine(renderer, (game->getPlayer()->pos.x) / 2,
                                 (game->getPlayer()->pos.y) / 2,
                                 pointX,
                                   pointY);
	SDL_RenderPresent(renderer);
    
    //if we ever want to destroy
    //SDL_DestroyTexture(textureWall);
    //SDL_FreeSurface(imageWall);
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

                if (mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] == 4 && kCollected == true) 
                {
                    mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] = 0;
                    std::cout << "Blue Door has been opened!" << std::endl;
                }

                else if (mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] == 5)
                {
                    game->setPlayer();
                    /* Vec3 position(0.5f * currentScene->getxAxis() - 300, 0.5f * currentScene->getyAxis(), 0.0f);
                     Vec3 velocity(cos(orientation) * 1.25, sin(orientation) * 1.25, 0.0f);
                     Vec3 acceleration(0.0f, 0.0f, 0.0f);*/
                    game->LoadScene(1);
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
            game->getPlayer()->orientation += 0.030;
            if (game->getPlayer()->orientation > 2 * PI)
            {
                game->getPlayer()->orientation -= 2 * PI;
            }
           
            game->getPlayer()->vel.x = cos(-game->getPlayer()->orientation) * 1.75f;
            game->getPlayer()->vel.y = sin(-game->getPlayer()->orientation) * 1.75f;
        }
        if (Keys.d == 1)
        {
            
            game->getPlayer()->orientation -= 0.030;
            if (game->getPlayer()->orientation < 0)
            {
                game->getPlayer()->orientation += 2 * PI;
            }

            game->getPlayer()->vel.x = cos(-game->getPlayer()->orientation) * 1.75f;
            game->getPlayer()->vel.y = sin(-game->getPlayer()->orientation) * 1.75f;
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
        // checking if within the keys range
        if (((game->getPlayer()->pos.y - 256) < 16) && ((game->getPlayer()->pos.y - 256) > -16) ) {
            if (((game->getPlayer()->pos.x - 256) < 16) && ((game->getPlayer()->pos.x - 256) > -16) && kCollected == false) {

                kCollected = true;
                std::cout << "Blue Key Acquired!" << std::endl;
            }

        }
}

void Scene1::Key()
{
    
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
            else if (mapWalls[y * mapWallsX + x] == 2) //red wall
            {
                SDL_SetRenderDrawColor(renderer, 240, 15, 15, 0);
            }
            else if (mapWalls[y * mapWallsX + x] == 4) //blue door
            {
                SDL_SetRenderDrawColor(renderer, 15, 15, 240, 0);
            }
            else if (mapWalls[y * mapWallsX + x] == 5) //green door
            {
                SDL_SetRenderDrawColor(renderer, 15, 240, 15, 0);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
            }
            SDL_Rect rect = { ((int)mapWallsS * x/2), ((int)mapWallsS * y/2), mapWallsS/2, mapWallsS/2 };
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

    ra = -player->orientation - DegToRad * 30;

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
            dof = 16;
        }
        while(dof < 16)
        {
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapWallsX + mx;
            if(mp > 0 && mp<mapWallsX * mapWallsY && mapWalls[mp] > 0)
            {
                hX = rx;
                hY = ry;
                disH = dist(player->pos.x, player->pos.y, hX, hY, ra);
                dof = 16;
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
            dof = 16;
        }
        while(dof < 16)
        {
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapWallsX + mx;
            if(mp > 0 && mp<mapWallsX * mapWallsY && mapWalls[mp] > 0)
            {
                vX = rx;
                vY = ry;
                disV = dist(player->pos.x, player->pos.y, vX, vY, ra);
                dof = 16;
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
            else if(mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 5)
            {
                color = Vec3(15, 240, 15);
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
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 5)
            {
                color = Vec3(15, 240, 15);
            }
            SDL_SetRenderDrawColor(renderer, color.x/1.6, color.y / 1.6, color.z / 1.6, 0);
            SDL_SetTextureColorMod(textureWall, color.x/1.6, color.y/1.6, color.z/1.6);
        }
       

        //3D Wall Drawing
        float ca = player->orientation + ra;

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
        float dy = lineO + lineH + 32;
        int texFloorX = player->pos.x / 2 + cos(ra * DegToRad) * 158 * 64 / dy / (cos(FixAng(ca - ra) * DegToRad));

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

        //Draw Floor
        SDL_Rect floor = { r * 4 + 530, dy, 4, 320 }; //under the walls
        SDL_Rect floorCrop = { texFloorX,
                               (int)game->getPlayer()->pos.y * 158 * 64/dy/ cos(FixAng(ca - ra) * DegToRad),
            1, 64};
        //SDL_RenderCopy(renderer, textureWall, &floorCrop, &floor);
        SDL_SetRenderDrawColor(renderer, 68, 69, 45, 0);
        SDL_RenderFillRect(renderer, &floor);

        //Draw Ceiling
        SDL_Rect ceiling = { r * 4 + 530, 0 , 4, lineO + 16 }; //under the walls
        SDL_Rect ceilingCrop = { texFloorX,
                               (int)game->getPlayer()->pos.y * 158 * 64 / dy / cos(FixAng(ca - ra) * DegToRad),
            1, 64 };
        //SDL_RenderCopy(renderer, textureWall, &floorCrop, &floor);
        SDL_SetRenderDrawColor(renderer, 208, 255, 240, 0);
        SDL_RenderFillRect(renderer, &ceiling);
        


        
        //SDL_RenderFillRect(renderer, &rect);
        //SDL_RenderDrawLine(renderer, r*8 + 530, lineO, r*8 + 530, lineH + lineO);
        //SDL_SetRenderDrawColor(renderer, 255, 15, 15, 0);
        //SDL_Rect TEST = { 128, 448, 64, 8 }; // if vertical we use y offset
        //SDL_RenderFillRect(renderer, &TEST);
       

        ra += DegToRad/2; // 60/120 = 1/2 * degrees
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

void drawFloors()
{

}

void Scene1::entityTick()
{
    //https://www.youtube.com/watch?v=eBFOjriHMc8

    int distPosX = (4 * 64) - game->getPlayer()->pos.x; //entity pos x - player pos.x
    int distPosY = (4 * 64) - game->getPlayer()->pos.y; //entity pos y - player pos.y

    int dist = sqrt((distPosX * distPosX) + (distPosY * distPosY));

    float angle = atan2(-distPosY, distPosX); //p = atan2(-h.y, h.x) * (180/PI)

    angle *= RADIANS_TO_DEGREES;
    //+180 to shift angle

    if (angle > 360)
    {
        angle -= 360;
    }
    else if (angle < 0)
    {
        angle += 360;
    }

    float yTemp = ((game->getPlayer()->orientation + (DEGREES_TO_RADIANS * 30)) * RADIANS_TO_DEGREES) - angle; //orientation + 30 degrees produces left most column, then subtract angle
    //TODO: prevent negative values
    //going y up and down on topdown map causes it to move position.
    //float yTemp = (game->getPlayer()->orientation - (DEGREES_TO_RADIANS * 30)) * RADIANS_TO_DEGREES + 30 - angle; //player_rot + (fov/2) - p

    if (angle > 270 && (game->getPlayer()->orientation - (DEGREES_TO_RADIANS * 30)) * RADIANS_TO_DEGREES < 90)
    {
        yTemp += 360;
    }
    if ((game->getPlayer()->orientation - (DEGREES_TO_RADIANS * 30)) * RADIANS_TO_DEGREES > 270 && angle < 90)
    {
        yTemp -= 360;
    }

    float xTemp = yTemp * 480 / 60.0; //480 = 120 rays x 4 thickness, 60 degrees
    
    //std::cout << "Angle to Item: " << angle << std::endl;
    std::cout << "Player Orientation: " << game->getPlayer()->orientation * RADIANS_TO_DEGREES << std::endl;
    std::cout << "YTemp: " << yTemp << std::endl;
    std::cout << "XTemp: " << xTemp << std::endl;

    int width = (240 / tan(30 * DEGREES_TO_RADIANS)) * 64;

    if(dist != 0)
    {
        width = (240/tan(30 * DEGREES_TO_RADIANS)) * 64 / dist; //480/2 60/2
    }

    if(xTemp + 530 >= 530 - width && !kCollected)
    {
        SDL_SetRenderDrawColor(renderer, 15, 15, 240, 0);
        SDL_Rect key = { xTemp + 530,320/2,  width,  width};
        SDL_RenderCopy(renderer,enemyTexture, NULL, &key);
    }
    
}

