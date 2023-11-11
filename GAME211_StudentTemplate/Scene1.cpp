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

<<<<<<< Updated upstream
	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {

	// Update player
=======
    //Items
    key = Entity(Vec2(192, 896), Vec2(0, 0), keyTexture);
    //healthItem = Entity(Vec2(96, 416), Vec2(0, 0));
    //ammoItem = Entity(Vec2(672, 928), Vec2(0, 0));
    
    // Set player image to PacMan

    skulker.push_back(new Enemy(3, Vec2(160, 160), Vec2(0, 0), skulkerTexture));
    skulker.push_back(new Enemy(3, Vec2(928, 96), Vec2(0, 0), skulkerTexture));
    skulker.push_back(new Enemy(3, Vec2(96, 672), Vec2(0, 0), skulkerTexture));
    skulker.push_back(new Enemy(3, Vec2(544, 224), Vec2(0, 0), skulkerTexture));

    predator.push_back(new Enemy(3, Vec2(544, 352), Vec2(0, 0), predatorTexture));

    predCanSee[0] = false;

    entities.reserve(predator.size() + skulker.size() + 1);

    for(Entity* entity: predator)
    {
        entities.push_back(entity);
    }
    for (Entity* entity : skulker)
    {
        entities.push_back(entity);
    }

    entities.push_back(&key);


    // Stuff We need for UI
    // Color and the font
    colorFont = { 255, 0, 255 };
    font = TTF_OpenFont("Roboto-Regular.ttf", 24);

    // itoa
    _itoa_s(player.getMaxHealth(), playerHealth, sizeof(playerHealth), 10); // Gets the Max health for health ttf
    _itoa_s(player.getMaxAmmo(), playerAmmo, sizeof(playerAmmo), 10); // Gets the Max ammo for ammo ttf

    // The health 
    health = TTF_RenderText_Solid(font, playerHealth, colorFont);
    healthTexture = SDL_CreateTextureFromSurface(renderer, health);
    healthName = TTF_RenderText_Solid(font, "Health: ", colorFont);
    healthNameTexture = SDL_CreateTextureFromSurface(renderer, healthName);

    // The ammo
    ammo = TTF_RenderText_Solid(font, playerAmmo, colorFont);
    ammoTexture = SDL_CreateTextureFromSurface(renderer, ammo);
    ammoName = TTF_RenderText_Solid(font, "Ammo: ", colorFont);
    ammoNameTexture = SDL_CreateTextureFromSurface(renderer, ammoName);

    // The key
    keyName = TTF_RenderText_Solid(font, "Key Collected: ", colorFont);
    keyNameTexture = SDL_CreateTextureFromSurface(renderer, keyName);
    cardKey = IMG_Load("CardKey.png");
    keyTexture = SDL_CreateTextureFromSurface(renderer, cardKey);

    // SDL Rect
    healthRect = { 10,30,health->w,health->h };
    healthNameRect = { 0,0,healthName->w,healthName->h };
    ammoRect = { 150, 30, ammo->w, ammo->h };
    ammoNameRect = { 130, 0, ammoName->w,ammoName->h };
    keyNameRect = { 250, 0, keyName->w, keyName->h };
    cardKeyRect = { 400,0, 64, 64 };

	return true;
}

void Scene1::OnDestroy() 
{
    SDL_DestroyTexture(textureWall);
    SDL_FreeSurface(imageWall);
    SDL_DestroyTexture(textureWall2);
    SDL_FreeSurface(imageWall2);
    SDL_DestroyTexture(textureFloor);
    SDL_FreeSurface(imageFloor);
    SDL_DestroyTexture(textureCeiling);
    SDL_FreeSurface(imageCeiling);
    SDL_DestroyTexture(textureDoor);
    SDL_FreeSurface(imageDoor);
    SDL_DestroyTexture(predatorTexture);
    SDL_FreeSurface(predatorSprite);
    SDL_DestroyTexture(skulkerTexture);
    SDL_FreeSurface(skulkerSprite);
    SDL_DestroyTexture(buffer);
    SDL_FreeSurface(surf);




}

void Scene1::Update(const float deltaTime) {
   // player.getCurrentHealth();
	// Update playerit
    if(shootGun)
    {
        timePassedGun += deltaTime;
        //shoot gun
        if(timePassedGun >= 0.08f)
        {
            currentGunFrame += 1;
            timePassedGun = 0;
        }
        if (currentGunFrame > 5)
        {
            currentGunFrame = 0;
            shootGun = false;
        }
    }
    if (hit)
    {
        timePassedHit += deltaTime;
        //shoot gun
        if (timePassedHit >= 0.06f)
        {
            fade += 50 * fadeDir;
            timePassedHit = 0;
        }
        if (fade > 200)
        {
            fade = 200;
            fadeDir = -1;
        }
        if(fade < 0 && fadeDir == -1)
        {
            fade = 0;
            fadeDir = 1;
            hit = false;
        }
    }

    

>>>>>>> Stashed changes
	game->getPlayer()->Update(deltaTime);
    HandleMovement();
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
	SDL_RenderClear(renderer); 

    drawMap2D();
    draw3D();
    
	// render the player
	game->RenderPlayer(1.0f);

<<<<<<< Updated upstream
	SDL_RenderPresent(renderer);
=======
    for(int i = 0; i < entities.size(); i++)
    {
        entityTick(entities[i], entities[i]->texture);
    }

    SDL_RenderCopy(renderer, textureGun[currentGunFrame], NULL, &gun);

    if (kCollected)
    {
        // key collect UI
        SDL_RenderCopy(renderer, keyTexture, NULL, &keyAcq);
    }

    SDL_Rect dmgFade = { 0,0, 960, 640 };
    SDL_SetRenderDrawColor(renderer, 180, 15, 15, fade);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &dmgFade);
    //SDL_RenderPresent(renderer);
    //if we ever want to destroy
    //SDL_DestroyTexture(textureWall);
    //SDL_FreeSurface(imageWall);



    // Renders the health and ammo of the UI
    SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);
    SDL_RenderCopy(renderer, healthNameTexture, NULL, &healthNameRect);
    SDL_RenderCopy(renderer, ammoTexture, NULL, &ammoRect);
    SDL_RenderCopy(renderer, ammoNameTexture, NULL, &ammoNameRect);
   // SDL_RenderCopy(renderer, keyNameTexture, NULL, &keyNameRect);

    // When key is collected it will display the cardkey png
    if (kCollected == true) {
        SDL_RenderCopy(renderer, keyNameTexture, NULL, &keyNameRect);
        SDL_RenderCopy(renderer, keyTexture, NULL, &cardKeyRect);

    }

    SDL_RenderPresent(renderer);
>>>>>>> Stashed changes
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
            if (map[ipy * mapX + ipx_add_xo] == 0)
            {
                game->getPlayer()->pos.x += game->getPlayer()->vel.x;

            }
            if (map[ipy_add_yo * mapX + ipx] == 0)
            {
                game->getPlayer()->pos.y += game->getPlayer()->vel.y;
            }
        }
        if (Keys.s == 1) //fix later
        {
            if (map[ipy * mapX + ipx_sub_xo] == 0)
            {
                game->getPlayer()->pos.x -= game->getPlayer()->vel.x;

            }
            if (map[ipy_sub_yo * mapX + ipx] == 0)
            {
                game->getPlayer()->pos.y -= game->getPlayer()->vel.y;
            }
        }
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
            else if (map[y * mapX + x] == 2)
            {
                SDL_SetRenderDrawColor(renderer, 240, 15, 15, 0);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
            }
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

    int test = 0;
    int currentGrid = 0;
    bool lastH = false;
    bool lastV = false;
    for(r=0; r<60; r++)
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
            mp = my * mapX + mx;
            if(mp > 0 && mp<mapX * mapY && map[mp] > 0)
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
            if(mp > 0 && mp<mapX * mapY && map[mp] > 0)
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
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
            {
                color = Vec3(240, 240, 240);
            }
            else if (mp > 0 && mp < mapX * mapY && map[mp] == 2)
            {
                color = Vec3(240, 15, 15);
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
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
            {
                color = Vec3(240, 240, 240);
            }
            else if (mp > 0 && mp < mapX * mapY && map[mp] == 2)
            {
                color = Vec3(240, 15, 15);
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

        float lineH = (mapS * 320) / disT;
        float lineO = 160 - lineH / 2;

        if(lineO < 0)
        {
            lineO = 0;
        }


        if(lineH > 320)
        {

            lineH = 320;
        }


        SDL_Rect rect = { r * 8 + 530, lineO, 8, lineH };
        SDL_Rect crop = { r,0, 8, lineH };
        
        SDL_RenderCopy(renderer, textureWall, &crop, &rect);
            
        //SDL_SetSurfaceColorMod(&rect, color.x, color.y, color.z);
        //SDL_SET
        //SDL_RenderFillRect(renderer, &rect);
        //SDL_RenderDrawLine(renderer, r*8 + 530, lineO, r*8 + 530, lineH + lineO);
       

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
