#include "Scene1.h"
#include <VMath.h>
#include <iostream>

using namespace std;



//typedef struct
//{
//    int w, a, d, s;
//}ButtonKeys; ButtonKeys Keys;

// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 1024.0f;
	yAxis = 512.0f;
    float orientation = 0.0f;
    //Items
    key = Entity(Vec2(192, 896), Vec2(0, 0));
    healthItem = Entity(Vec2(96, 416), Vec2(0, 0));
    ammoItem = Entity(Vec2(672, 928), Vec2(0, 0));
    //Enemies
    skulker1 = Enemy(3, Vec2(160, 160), Vec2(0, 0));


    //Player
    player = Player(10, 6, orientation, Vec2(0.8f * getxAxis() - 300, 1.6f * getyAxis()), Vec2(cos(orientation) * 1.75, sin(orientation) * 1.75));


}


Scene1::~Scene1(){
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
    kCollected = false;
    aCollected = false;
    hCollected = false;

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
  /*  Entity Key(Vec2(0,0),Vec2(0,0));
    Player player(10, 0, 0, Vec2(10, 10), Vec2(0, 0));*/
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to PacMan

    skulker.push_back(new Enemy(3, Vec2(160, 160), Vec2(0, 0)));
    skulker.push_back(new Enemy(3, Vec2(928, 96), Vec2(0, 0)));
    skulker.push_back(new Enemy(3, Vec2(96, 672), Vec2(0, 0)));
    skulker.push_back(new Enemy(3, Vec2(544, 224), Vec2(0, 0)));

	SDL_Surface* image;
    
	SDL_Texture* texture;

	image = IMG_Load("pacman.png");
    imageWall = IMG_Load("wallTest.png");
    imageWall2 = IMG_Load("wallTest2.png");
    imageDoor = IMG_Load("door.png");
    imageDoor2 = IMG_Load("door2.png");
    enemySprite = IMG_Load("CardKey.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
    textureWall = SDL_CreateTextureFromSurface(renderer, imageWall);
    textureWall2 = SDL_CreateTextureFromSurface(renderer, imageWall2);
    textureDoor = SDL_CreateTextureFromSurface(renderer, imageDoor);
    textureDoor2 = SDL_CreateTextureFromSurface(renderer, imageDoor2);
    enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySprite);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	return true;
}

void Scene1::OnDestroy() 
{
    SDL_DestroyTexture(textureWall);
    SDL_FreeSurface(imageWall);
    SDL_DestroyTexture(textureWall2);
    SDL_FreeSurface(imageWall2);
    SDL_DestroyTexture(textureDoor);
    SDL_FreeSurface(imageDoor);
    SDL_DestroyTexture(enemyTexture);
    SDL_FreeSurface(enemySprite);
}

void Scene1::Update(const float deltaTime) {
   // player.getCurrentHealth();
	// Update playerit
	game->getPlayer()->Update(deltaTime);
    HandleMovement();
    player.playerUpdate(deltaTime);
    for (int i = 0; i < skulker.size();i++) {
        skulker[i]->updatePos(player.getPosition());
    }
    // 4/2 because of map size(4,4) = x,y
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
        SDL_RenderCopy(renderer, enemyTexture, NULL, &keyAcq);
    }
    else
    {
        //key map dot
        SDL_SetRenderDrawColor(renderer, 15, 240, 15, 0);
        SDL_RenderFillRect(renderer, &keyMap);
    }

    if (aCollected==false)
    {
        //ammo map dot
        SDL_Rect ammoMap = { ammoItem.getPosition().x / 2, ammoItem.getPosition().y / 2, 4, 4 };
        SDL_SetRenderDrawColor(renderer, 255, 255, 15, 0);
        SDL_RenderFillRect(renderer, &ammoMap);
    }

    if (hCollected==false)
    {
        //health map dot
        SDL_Rect healthMap = { healthItem.getPosition().x / 2, healthItem.getPosition().y / 2, 4, 4 };
        SDL_SetRenderDrawColor(renderer, 15, 15, 240, 0);
        SDL_RenderFillRect(renderer, &healthMap);
    }
  
    //Enemy map dots
    for (int i = 0; i < skulker.size(); i++) {
        SDL_Rect skulkerMap = { skulker[i]->getPosition().x / 2, skulker[i]->getPosition().y / 2, 4, 4 };
        SDL_SetRenderDrawColor(renderer, 240, 15, 15, 0);
        SDL_RenderFillRect(renderer, &skulkerMap);
    }

	// render the player
    SDL_Rect playerPos = { (player.getPosition().x - 2)/2, (player.getPosition().y - 2)/2, 4, 4 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 15, 0); 
    SDL_RenderFillRect(renderer, &playerPos);

    // render direction on map
    int pointX = (player.getPosition().x) / 2;
    pointX += cos(-player.getOrientation()) * (((player.getPosition().x) / 2 + 8) - (player.getPosition().x) / 2) - sin(-player.getOrientation()) * (0);
    int pointY = (player.getPosition().y) / 2;
    pointY += sin(-player.getOrientation()) * (((player.getPosition().x) / 2 + 8) - (player.getPosition().x) / 2) + cos(-player.getOrientation()) * (0);
    SDL_RenderDrawLine(renderer, (player.getPosition().x) / 2,
                                 (player.getPosition().y) / 2,
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
                player.a = 1; 
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_D) 
            {
                player.d = 1;
            }

            if (event.key.keysym.scancode == SDL_SCANCODE_W) //fix later
            {
                player.w = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_S) //fix later
            {
                player.s = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_E)
            {
                //check if interactable is close enough in front of us
                int xo = 0, yo = 0;
                
                if (player.getVelocity().x < 0)
                {
                    xo = -25;
                }
                else
                {
                    xo = 25;
                }
                if (player.getVelocity().y < 0)
                {
                    yo = -25;
                }
                else
                {
                    yo = 25;
                }
                int ipx = player.getPosition().x / 64.0, ipx_add_xo = (player.getPosition().x + xo) / 64.0;
                int ipy = player.getPosition().y / 64.0, ipy_add_yo = (player.getPosition().y + yo) / 64.0;

                //check what grid value interactable has
                //4 = regular door
                if (mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] == 4 ) 
                {
                    game->getSoundEngine()->play2D("door2.wav", false);
                    mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] = 0;
                }
                //5 = door locked by green keycard
                else if (mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] == 5 && kCollected == true)
                {
                    game->getSoundEngine()->play2D("door2.wav", false);
                    player.w = 0;
                    player.a = 0;
                    player.s = 0;
                    player.d = 0;
                    //load menu
                    game->LoadScene(2);
                }
                else if (mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] == 5 && kCollected == false)
                {
                    game->getSoundEngine()->play2D("denied.wav", false);
                }

            }
        }
        if(event.type == SDL_KEYUP)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_A)
            {
                player.a = 0;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_D)
            {
                player.d = 0;
            }

            if (event.key.keysym.scancode == SDL_SCANCODE_W) 
            {
                player.w = 0;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_S) 
            {
                player.s = 0;
            }
        }
}

void Scene1::HandleMovement()
{
    if (player.a == 1)
    {
        player.setOrientation(player.getOrientation() + 0.030);
        if (player.getOrientation() > 2 * PI)
        {
            player.setOrientation(player.getOrientation() - 2 * PI);
        }


        player.setvelocity(Vec2(cos(-player.getOrientation()) * 1.75f, sin(-player.getOrientation()) * 1.75));
    }
        if (player.d == 1)
        {

            player.setOrientation(player.getOrientation() - 0.030);
            if (player.getOrientation() < 0)
            {
                player.setOrientation(player.getOrientation() + 2 * PI);
            }
            player.setvelocity(Vec2(cos(-player.getOrientation()) * 1.75f, sin(-player.getOrientation()) * 1.75));
        }

        int xo = 0, yo = 0;

        if (player.getVelocity().x < 0)
        {
            xo = -20;
        }
        else
        {
            xo = 20;
        }
        if (player.getVelocity().y < 0)
        {
            yo = -20;
        }
        else
        {
            yo = 20;
        }
        int ipx = player.getPosition().x / 64.0, ipx_add_xo = (player.getPosition().x + xo) / 64.0, ipx_sub_xo = (player.getPosition().x - xo) / 64.0;
        int ipy = player.getPosition().y / 64.0, ipy_add_yo = (player.getPosition().y + yo) / 64.0, ipy_sub_yo = (player.getPosition().y - yo) / 64.0;
        if (player.w == 1) 
        {
            if (mapWalls[ipy * mapWallsX + ipx_add_xo] == 0)
            {

                player.setPosition(Vec2(player.getPosition().x + player.getVelocity().x, player.getPosition().y));
            }
            if (mapWalls[ipy_add_yo * mapWallsX + ipx] == 0)
            {
                player.setPosition(Vec2(player.getPosition().x, player.getPosition().y + player.getVelocity().y));
            }
        }
        if (player.s == 1) 
        {
            if (mapWalls[ipy * mapWallsX + ipx_sub_xo] == 0)
            {
                player.setPosition(Vec2(player.getPosition().x - player.getVelocity().x, player.getPosition().y));

            }
            if (mapWalls[ipy_sub_yo * mapWallsX + ipx] == 0)
            {
                player.setPosition(Vec2(player.getPosition().x, player.getPosition().y - player.getVelocity().y));
            }
        }

        //key item collision
        if(player.collField(key.getPosition()) && kCollected == false)
        {

                kCollected = true;
                game->getSoundEngine()->play2D("beep.wav", false);
                std::cout << "Green Key Acquired!" << std::endl;
        }

        //Enemy attack check

        for (int i = 0; i < skulker.size(); i++) {
            if (player.collField(skulker[i]->getPosition())) {

                player.subHealth(1);
                // cout << "player hit!" << endl;

            }
        }
        if (player.collField(healthItem.getPosition()) && hCollected == false) {
            hCollected = true;
            player.addHealth(1);
            cout << "player healed!" << endl;
            cout << "player health = " << player.getCurrentHealth() << endl;
        }
        if (player.collField(ammoItem.getPosition()) && aCollected == false) {
            aCollected = true;
            player.addAmmo(3);
            cout << "player picked up ammo!" << endl;
            cout << "player ammo = " << player.getAmmo() << endl;
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

float Scene1::dist(float ax, float ay, float bx, float by, float ang)
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

   // PlayerBody* player = game->getPlayer();
    SDL_Texture* tempTex = textureWall;


    ra = -player.getOrientation() - DegToRad * 30;

    if(ra < 0)
    {
        ra += 2 * PI;
    }
    if(ra > 2*PI)
    {
        ra -= 2 * PI;
    }

    //480 rays for every x value in the 480x320 screen
    for(r=0; r<480; r++)
    {
        disT = 10000000;
        // Horizontal Line Check
        dof = 0;
        float disH = 100000000;
        float hX = player.getPosition().x;
        float hY = player.getPosition().y;
        float aTan = -1 / tan(ra);
        if(ra > PI)
        {
            ry = (int)(player.getPosition().y / 64) * 64 - 0.0001;
            rx = (player.getPosition().y - ry) * aTan + player.getPosition().x;
            yo = -64;
            xo = -yo * aTan;
        }
        if (ra < PI)
        {
            ry = (int)(player.getPosition().y / 64) * 64 + 64;
            rx = (player.getPosition().y - ry) * aTan + player.getPosition().x;
            yo = 64;
            xo = -yo * aTan;
        }
        if(ra ==0 || ra ==PI)
        {
            rx = player.getPosition().x;
            ry = player.getPosition().y;
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
                disH = dist(player.getPosition().x, player.getPosition().y, hX, hY, ra);
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
        float vX = player.getPosition().x;
        float vY = player.getPosition().y;
        float nTan = -tan(ra);
        if(ra > P2 && ra < P3)
        {
            rx = (int)(player.getPosition().x / 64) * 64 - 0.0001;
            ry = (player.getPosition().x - rx) * nTan + player.getPosition().y;
            xo = -64;
            yo = -xo * nTan;
        }
        if (ra < P2 || ra > P3)
        {
            rx = (int)(player.getPosition().x / 64) * 64 + 64;
            ry = (player.getPosition().x - rx) * nTan + player.getPosition().y;
            xo = 64;
            yo = -xo * nTan;
        }
        if(ra ==0 || ra ==PI)
        {
            rx = player.getPosition().x;
            ry = player.getPosition().y;
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
                disV = dist(player.getPosition().x, player.getPosition().y, vX, vY, ra);
                dof = 16;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        //see if closest collision is the vertical or horizontal line, if horizontal darken the texture
        if(disV<disH)
        {
            rx = vX;
            ry = vY;
            disT = disV;
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapWallsX + mx;
            color = Vec3(240, 240, 240);
            if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 1)
            {
                tempTex = textureWall;
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 2)
            {
                tempTex = textureWall2;
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 4)
            {
                tempTex = textureDoor;
            }
            else if(mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 5)
            {
                tempTex = textureDoor2;
            }
            //SDL_SetRenderDrawColor(renderer,color.x, color.y, color.z, 0);
            SDL_SetTextureColorMod(tempTex, color.x, color.y, color.z);
        }
        else if (disH<disV)
        {
            rx = hX;
            ry = hY;
            disT = disH;
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapWallsX + mx;
            color = Vec3(240, 240, 240);
            if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 1)
            {
                tempTex = textureWall;
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 2)
            {
                tempTex = textureWall2;
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 4)
            {
                tempTex = textureDoor;
            }
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 5)
            {
                tempTex = textureDoor2;
            }
            //SDL_SetRenderDrawColor(renderer, color.x/1.6, color.y / 1.6, color.z / 1.6, 0);
            SDL_SetTextureColorMod(tempTex, color.x/1.6, color.y/1.6, color.z/1.6);
        }
       

        //3D Wall Drawing
        float ca = player.getOrientation() + ra;

        if (ca < 0)
        {
            ca += 2 * PI;
        }
        if (ca > 2 * PI)
        {
            ca -= 2 * PI;
        }

        disT *= cos(ca); //correct fisheye

        zBuffer[r] = disT; //add the final hit distance into our zBuffer

        float lineH = (mapWallsS * 320) / disT; //line we draw to mimic the 3d view using the wall size, screen height and distance.
        float lineO = 160 - lineH / 2; //offset so we don't start from the very top of the screen

        //make our line into a rect
        SDL_Rect rect = { r + 530, lineO + 16, 1, lineH + 16 };

        //where textures go
        int texVX = (ry / 64 - my) * (64);
        int texHX = (rx / 64 - mx) * (64);
        float dy = lineO + lineH + 32;
        int texFloorX = player.getPosition().x / 2 + cos(ra * DegToRad) * 158 * 64 / dy / (cos(FixAng(ca - ra) * DegToRad));

        if (disV < disH) //check if vertical or horizontal
        {
            SDL_Rect crop = { texVX, 0, 1, 64}; // if vertical we use y offset
            SDL_RenderCopy(renderer, tempTex, &crop, &rect);
        }
        else
        {
            SDL_Rect crop = { texHX, 0, 1, 64}; // if horizontal we use x offset
            SDL_RenderCopy(renderer, tempTex, &crop, &rect);
        }


        //Draw Floor (everything after the wall is done)
        SDL_Rect floor = { r + 530, dy, 1, 320 }; //under the walls
        SDL_Rect floorCrop = { texFloorX,
                               (int)game->getPlayer()->pos.y * 158 * 64/dy/ cos(FixAng(ca - ra) * DegToRad),
            1, 64};
        SDL_SetRenderDrawColor(renderer, 25, 27, 63, 0);
        SDL_RenderFillRect(renderer, &floor);

        //Draw Ceiling (everything before the wall is done)
        SDL_Rect ceiling = { r + 530, 0 , 1, lineO + 16 }; //under the walls
        SDL_Rect ceilingCrop = { texFloorX,
                               (int)game->getPlayer()->pos.y * 158 * 64 / dy / cos(FixAng(ca - ra) * DegToRad),
            1, 64 };
        SDL_SetRenderDrawColor(renderer, 7, 12, 29, 0);
        SDL_RenderFillRect(renderer, &ceiling);
        
        //SDL_RenderFillRect(renderer, &rect);
        //SDL_RenderDrawLine(renderer, r*8 + 530, lineO, r*8 + 530, lineH + lineO);
       
        //degree change 60 (fov)  / 480 (number of rays) = 1/8 degree change per ray
        ra += DegToRad/8;
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

void Scene1::drawFloors()
{

}

void Scene1::entityTick()
{
    //Major help from:
    //https://www.youtube.com/watch?v=eBFOjriHMc8
    //https://wynnliam.github.io/raycaster/news/tutorial/2019/04/03/raycaster-part-02.html

    //first find distance between key and player
    int distPosX = (key.getPosition().x) - player.getPosition().x; //entity pos x - player pos.x
    int distPosY = (key.getPosition().y) - player.getPosition().y; //entity pos y - player pos.y

    int dist = sqrt((distPosX * distPosX) + (distPosY * distPosY));

    //then angle (also considering pos y is downward)
    float angle = atan2(-distPosY, distPosX); //p = atan2(-h.y, h.x) * (180/PI)

    angle *= RADIANS_TO_DEGREES;

    //correct angle if it's out of bounds
    if (angle > 360)
    {
        angle -= 360;
    }
    else if (angle < 0)
    {
        angle += 360;
    }

    float yTemp = ((player.getOrientation() + (DEGREES_TO_RADIANS * 30)) * RADIANS_TO_DEGREES) - angle; //orientation + 30 degrees produces left most column, then subtract angle
    

    if (angle > 270 && (player.getOrientation() - (DEGREES_TO_RADIANS * 30)) * RADIANS_TO_DEGREES < 90)
    {
        yTemp += 360;
    }
    if ((player.getOrientation() - (DEGREES_TO_RADIANS * 30)) * RADIANS_TO_DEGREES > 270 && angle < 90)
    {
        yTemp -= 360;
    }

    float xTemp = yTemp * 480 / 60.0; //480 rays at 1 thickness, 60 degrees

    int width = (240 / tan(30 * DEGREES_TO_RADIANS)); // (projection_plane.width / 2) / tan(fov / 2) so 480/2 / tan(60/2)

    if (dist != 0)
    {
        width = (240 / tan(30 * DEGREES_TO_RADIANS)) * 64 / dist; //480/2 60/2
        width = width / 480.0 * 120; //scale
    }

    if (!kCollected)
    {
        //we split up the item sprite so that we can compare the distance of each slice to the distance of each wall
        //if wall is closer, we don't render that part of the sprite!
        int drawStartX = -width / 2 + xTemp;
        if (drawStartX < 0) drawStartX = 0;
        int drawEndX = width / 2 + xTemp;
        if (drawEndX >= 480) drawEndX = 480;
        for (int i = drawStartX; i <= drawEndX; i++)
        {
            if (530 + (i) >= 530 && 530 + (i) <= 1010)
            {
                int rayPos = (int)((((530 + (i)) - 530) / 480.0) * 480); //530 is where the screen starts at the moment
                //if direction of this hits wall of distance greater than sprite
                if (dist < zBuffer[rayPos]) {
                    SDL_Rect key = { 530 + i, 320 / 2 + 48, 1,width };
                    SDL_Rect crop = { int(i - (-width / 2 + xTemp)) * 64 / width,0,  1,  64 };
                    //SDL_SetRenderDrawColor(renderer, 15, 15, 240, 0);
                    //SDL_RenderFillRect(renderer, &key);
                    SDL_RenderCopy(renderer, enemyTexture, &crop, &key);
                }
            }

        }
    }

}

