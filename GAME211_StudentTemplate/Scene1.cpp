#include "Scene1.h"
#include <VMath.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "Entity.h"
#include <cmath>
#include <irrKlang.h>


using namespace std;

using namespace irrklang;


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
    //Player
    player = Player(10, 6, orientation, Vec2(0.8f * getxAxis() - 300, 1.6f * getyAxis()), Vec2(cos(orientation) * 1.75, sin(orientation) * 1.75));

}


Scene1::~Scene1(){
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
    SDL_RenderSetScale(renderer, 2, 2);
    surf = SDL_CreateRGBSurface(0, 480, 320, 32, 0, 0, 0, 0);
    pixels = (Uint32*)surf->pixels;
    buffer = SDL_CreateTextureFromSurface(renderer, surf);

    kCollected = false;

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
  /*  Entity Key(Vec2(0,0),Vec2(0,0));
    Player player(10, 0, 0, Vec2(10, 10), Vec2(0, 0));*/
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* image;
    
	SDL_Texture* texture;

	image = IMG_Load("pacman.png");
    imageWall = IMG_Load("wallTest.png");
    imageWall2 = IMG_Load("wallTest2.png");
    imageDoor = IMG_Load("door.png");
    imageDoor2 = IMG_Load("door2.png");
    keySprite = IMG_Load("CardKey.png");
    imageFloor = IMG_Load("floor2.png");
    imageCeiling = IMG_Load("ceiling.png");
    predatorSprite = IMG_Load("Blinky.png");
    skulkerSprite = IMG_Load("Blinky2.png");
    stalkerSprite = IMG_Load("Blinky.png");
    healthSprite = IMG_Load("CardKey.png");
    ammoSprite = IMG_Load("CardKey.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
    textureWall = SDL_CreateTextureFromSurface(renderer, imageWall);
    textureWall2 = SDL_CreateTextureFromSurface(renderer, imageWall2);
    textureDoor = SDL_CreateTextureFromSurface(renderer, imageDoor);
    textureDoor2 = SDL_CreateTextureFromSurface(renderer, imageDoor2);
    keyTexture = SDL_CreateTextureFromSurface(renderer, keySprite);
    textureFloor = SDL_CreateTextureFromSurface(renderer, imageFloor);
    textureCeiling = SDL_CreateTextureFromSurface(renderer, imageCeiling);
    for (int i = 0; i < 6; i++) {
        std::string gunFrame = "Gun/gun" + std::to_string(i) + ".png";
        SDL_Surface* gunSurf = IMG_Load(gunFrame.c_str());
        SDL_Texture* gunTexture = SDL_CreateTextureFromSurface(renderer, gunSurf);
        SDL_FreeSurface(gunSurf);
        textureGun[i] = gunTexture;
    }
    currentGunFrame = 0;
    predatorTexture = SDL_CreateTextureFromSurface(renderer, predatorSprite);
    skulkerTexture = SDL_CreateTextureFromSurface(renderer, skulkerSprite);
    stalkerTexture = SDL_CreateTextureFromSurface(renderer, stalkerSprite);
    ammoTexture = SDL_CreateTextureFromSurface(renderer, ammoSprite);
    healthTexture = SDL_CreateTextureFromSurface(renderer, healthSprite);


 
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

    //Items
    key = Entity(Vec2(192, 896), Vec2(0, 0), keyTexture);
    //healthItem = Entity(Vec2(96, 416), Vec2(0, 0));
    //ammoItem = Entity(Vec2(672, 928), Vec2(0, 0));
    
    ammo.push_back(new Entity(Vec2(672, 928), Vec2(0, 0), ammoTexture));
    health.push_back(new Entity(Vec2(96, 416), Vec2(0, 0), healthTexture));

    skulker.push_back(new Enemy(3, Vec2(160, 160), Vec2(0, 0), skulkerTexture));
    skulker.push_back(new Enemy(3, Vec2(928, 96), Vec2(0, 0), skulkerTexture));
    skulker.push_back(new Enemy(3, Vec2(96, 672), Vec2(0, 0), skulkerTexture));
    skulker.push_back(new Enemy(3, Vec2(544, 224), Vec2(0, 0), skulkerTexture));

    predator.push_back(new Enemy(3, Vec2(224, 800), Vec2(0, 0),predatorTexture));

    stalker.push_back(new Enemy(3, Vec2(800, 544), Vec2(0, 0),stalkerTexture));


    entities.reserve(predator.size() + skulker.size() + stalker.size()+ ammo.size() + health.size() + 1);

    for(Entity* entity: predator)
    {
        entities.push_back(entity);
    }
    for (Entity* entity : skulker)
    {
        entities.push_back(entity);
    }
    for (Entity* entity : stalker)
    {
        entities.push_back(entity);
    }
    for (Entity* entity : ammo)
    {
        entities.push_back(entity);
    }
    for (Entity* entity : health)
    {
        entities.push_back(entity);
    }
    entities.push_back(&key);

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
    SDL_DestroyTexture(stalkerTexture);
    SDL_FreeSurface(stalkerSprite);
    SDL_DestroyTexture(ammoTexture);
    SDL_FreeSurface(ammoSprite);
    SDL_DestroyTexture(healthTexture);
    SDL_FreeSurface(healthSprite);

    SDL_DestroyTexture(buffer);
    SDL_FreeSurface(surf);
}

float CalculateDistance(const Vec2& point1, const Vec2& point2) {
    float deltaX = point1.x - point2.x;
    float deltaY = point1.y - point2.y;
    return sqrt(deltaX * deltaX + deltaY * deltaY);
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
	game->getPlayer()->Update(deltaTime);
    HandleMovement();
    player.playerUpdate(deltaTime);
    for (int i = 0; i < skulker.size();i++) {
     //   skulker[i]->updatePos(player.getPosition());
    }
    // 4/2 because of map size(4,4) = x,y


    ISoundSource* skulkerSoundSource = nullptr;
    ISoundSource* predatorSoundSource = nullptr;
    ISoundSource* stalkerSoundSource = nullptr;

    if (game->getSoundEngine()) {

        // Define sound sources for enemy types.
        if (!game->getSoundEngine()->getSoundSource("skulker.wav"))
            skulkerSoundSource = game->getSoundEngine()->addSoundSourceFromFile("skulker.wav");

        if (!game->getSoundEngine()->getSoundSource("predator.wav"))
            predatorSoundSource = game->getSoundEngine()->addSoundSourceFromFile("predator.wav");

        if (!game->getSoundEngine()->getSoundSource("stalker.wav"))
            stalkerSoundSource = game->getSoundEngine()->addSoundSourceFromFile("stalker.wav");

    }
    
    vec3df listenerPosition(player.getPosition().x, player.getPosition().y, 0);

    float maxHearingDistance = 100.0f; // You can adjust this value as needed


    if (predatorSoundSource) {
        for (int i = 0; i < predator.size(); i++) {
            if (predator[i]->VisionCheck(player, 25) && EnemyMoveUpate(predator[i])) {

                Vec2 enemyPosition = predator[i]->getPosition();
                float distance = CalculateDistance(enemyPosition, player.getPosition());

                if (predator[i]->collField(player.getPosition())) {
                    float volume = 1.0f - (distance / maxHearingDistance);

                    game->getSoundEngine()->play3D(predatorSoundSource, vec3df(enemyPosition.x, enemyPosition.y, 0), false, false, true, volume);
                }

                predator[i]->updatePos(player.getPosition());


            }
        }
    }
    if (stalkerSoundSource) {
        for (int i = 0; i < stalker.size(); i++) {
            if (!stalker[i]->VisionCheck(player, 30) && EnemyMoveUpate(stalker[i])) {

                Vec2 enemyPosition = stalker[i]->getPosition();
                float distance = CalculateDistance(enemyPosition, player.getPosition());

                if (stalker[i]->collField(player.getPosition())) {
                    float volume = 1.0f - (distance / maxHearingDistance);

                    game->getSoundEngine()->play3D(stalkerSoundSource, vec3df(enemyPosition.x, enemyPosition.y, 0), false, false, true, volume);
                }

                stalker[i]->updatePos(player.getPosition());
            }
        }
    }

    if (skulkerSoundSource) {
        for (int i = 0; i < skulker.size(); i++) {
            if (EnemyMoveUpate(skulker[i])) {
                Vec2 enemyPosition = skulker[i]->getPosition();
                float distance = CalculateDistance(enemyPosition, player.getPosition());

                if (skulker[i]->collField(player.getPosition())) {
                    float volume = 1.0f - (distance / maxHearingDistance);

                    game->getSoundEngine()->play3D(skulkerSoundSource, vec3df(enemyPosition.x, enemyPosition.y, 0), false, false, true, volume);
                }
                skulker[i]->updatePos(player.getPosition());
            }

        }
    }
}



void Scene1::Render() {
    SDL_RenderClear(renderer);

    drawFloors();
    SDL_UpdateTexture(buffer, NULL, surf->pixels, surf->pitch);
    SDL_RenderCopy(renderer, buffer, NULL, NULL);
    draw3D();
    
    std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) 
    {
        return sortByDistance(a, b);
    });

    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->getExist() == true) {
        entityTick(entities[i], entities[i]->texture);
        }
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
    SDL_RenderPresent(renderer);
    //if we ever want to destroy
    //SDL_DestroyTexture(textureWall);
    //SDL_FreeSurface(imageWall);
}

void Scene1::HandleEvents(const SDL_Event& event)
{

        if (event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.scancode == SDL_SCANCODE_LCTRL)
            {
                if(!shootGun)
                {
                    if (player.getAmmo() > 0) {

                        for (int i = 0; i < predator.size(); i++) {
                            if (predator[i]->VisionCheck(player, 3) && EnemyMoveUpate(predator[i])) {

                              predator[i]->subtractHealth(1);
                            }
                        }


                        for (int i = 0; i < stalker.size(); i++) {
                            if (stalker[i]->VisionCheck(player, 3) && EnemyMoveUpate(stalker[i])) {

                                stalker[i]->subtractHealth(1);
                            }
                        }


                        for (int i = 0; i < skulker.size(); i++) {
                            if (skulker[i]->VisionCheck(player, 3) && EnemyMoveUpate(skulker[i])) {
                             
                                skulker[i]->subtractHealth(1);
                            }

                        }

                        game->getSoundEngine()->play2D("pistol_shot.wav", false);
                        shootGun = true;
                        player.subAmmo(1);
                    }
                } 
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) 
            {
                player.a = 1; 
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) 
            {
                player.d = 1;
            }

            if (event.key.keysym.scancode == SDL_SCANCODE_UP) //fix later
            {
                player.w = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) //fix later
            {
                player.s = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
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
                else if (kCollected == true && mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] == 5)
                {
                    game->getSoundEngine()->play2D("door2.wav", false);
                    player.w = 0;
                    player.a = 0;
                    player.s = 0;
                    player.d = 0;
                    //load menu
                    game->LoadScene(2);
                }
                else if (kCollected == false && mapWalls[ipy_add_yo * mapWallsX + ipx_add_xo] == 5)
                {
                    game->getSoundEngine()->play2D("denied.wav", false);
                }

            }
        }
        if(event.type == SDL_KEYUP)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
            {
                player.a = 0;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
            {
                player.d = 0;
            }

            if (event.key.keysym.scancode == SDL_SCANCODE_UP) 
            {
                player.w = 0;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) 
            {
                player.s = 0;
            }
        }
}
bool Scene1::EnemyMoveUpate(Enemy* enemy_)
{
    int r, mx, my, mp, dof;

    float rx, ry;
    float xo = 0;
    float yo = 0;

    float disT;
    float pX = player.getPosition().x;
    float pY = player.getPosition().y;

    float eX, eY, orDeg, tes, sra, pra;


    eX = enemy_->getPosition().x;
    eY = enemy_->getPosition().y;

    tes = atan2(eY - pY, pX - eX);
    /*  orDeg = tes * RADIANS_TO_DEGREES;
      cout << orDeg << endl;*/
    enemy_->setOrientation(tes);

    ///////////////////
    disT = 10000000;
    // Horizontal Line Check
    sra = -enemy_->getOrientation();
    dof = 0;
    float disH = 100000000;
    float ehx = enemy_->getPosition().x;
    float ehy = enemy_->getPosition().y;
    float aTan = -1 / tan(sra);

    // sra = -enemy_->getOrientation() - DegToRad * 30;

    if (sra < 0)
    {
        sra += 2 * PI;
    }
    if (sra > 2 * PI)
    {
        sra -= 2 * PI;
    }

    if (sra > PI)
    {
        ry = (int)(enemy_->getPosition().y / 64) * 64 - 0.0001;
        rx = (enemy_->getPosition().y - ry) * aTan + enemy_->getPosition().x;
        yo = -64;
        xo = -yo * aTan;
    }
    if (sra < PI)
    {
        ry = (int)(enemy_->getPosition().y / 64) * 64 + 64;
        rx = (enemy_->getPosition().y - ry) * aTan + enemy_->getPosition().x;
        yo = 64;
        xo = -yo * aTan;
    }
    if (sra == 0 || sra == PI)
    {
        rx = enemy_->getPosition().x;
        ry = enemy_->getPosition().y;
        dof = 16;
    }
    while (dof < 16)
    {
        mx = rx / 64;
        my = ry / 64;
        mp = my * mapWallsX + mx;
        if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] > 0)
        {
            ehx = rx;
            ehy = ry;
            disH = dist(enemy_->getPosition().x, enemy_->getPosition().y, ehx, ehy);
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
    float vX = enemy_->getPosition().x;
    float vY = enemy_->getPosition().y;
    float nTan = -tan(sra);
    if (sra > P2 && sra < P3)
    {
        rx = (int)(enemy_->getPosition().x / 64) * 64 - 0.0001;
        ry = (enemy_->getPosition().x - rx) * nTan + enemy_->getPosition().y;
        xo = -64;
        yo = -xo * nTan;
    }
    if (sra < P2 || sra > P3)
    {
        rx = (int)(enemy_->getPosition().x / 64) * 64 + 64;
        ry = (enemy_->getPosition().x - rx) * nTan + enemy_->getPosition().y;
        xo = 64;
        yo = -xo * nTan;
    }
    if (sra == 0 || sra == PI)
    {
        rx = enemy_->getPosition().x;
        ry = enemy_->getPosition().y;
        dof = 16;
    }
    while (dof < 16)
    {
        mx = rx / 64;
        my = ry / 64;
        mp = my * mapWallsX + mx;
        if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] > 0)
        {
            vX = rx;
            vY = ry;
            disV = dist(enemy_->getPosition().x, enemy_->getPosition().y, vX, vY);
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
    if (disV < disH)
    {
        rx = vX;
        ry = vY;
        disT = disV;
        mx = rx / 64;
        my = ry / 64;
        mp = my * mapWallsX + mx;


    }
    else if (disH < disV)
    {
        rx = ehx;
        ry = ehy;
        disT = disH;
        mx = rx / 64;
        my = ry / 64;
        mp = my * mapWallsX + mx;

    }

    if (disT > dist(eX, eY, pX, pY)) {
        return true;

    }
    else {
        return false;
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
        // Define variables for time measurement.
        static std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
        static std::chrono::duration<double> frameDuration = std::chrono::duration<double>::zero();

        // Calculate deltaTime.
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        frameDuration = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - previousTime);
        previousTime = currentTime;
        float deltaTime = static_cast<float>(frameDuration.count());
        static bool isLoopingSoundPlaying = false;
        static float timeSinceLastSound = 0.0f;

        if (player.a == 1 || player.d == 1 || player.w == 1 || player.s == 1)
        {
            if (!isLoopingSoundPlaying)
            {
                // Start playing the sound when a movement key is pressed and the looping sound is not already playing.
                game->getSoundEngine()->play2D("walking.wav", true);
                isLoopingSoundPlaying = true;
            }

            // Increment the timer while a movement key is held.
            timeSinceLastSound += deltaTime;

            if (timeSinceLastSound >= 4.0f)
            {
                // Restart the sound loop every 4 seconds.
                irrklang::ISoundSource* loopingSoundSource = game->getSoundEngine()->getSoundSource("walking.wav");
                if (loopingSoundSource)
                {
                    game->getSoundEngine()->stopAllSoundsOfSoundSource(loopingSoundSource);
                    game->getSoundEngine()->play2D("walking.wav", true);
                }
                timeSinceLastSound = 0.0f;
            }
        }
        else
        {
            if (isLoopingSoundPlaying)
            {
                irrklang::ISoundSource* loopingSoundSource = game->getSoundEngine()->getSoundSource("walking.wav");
                if (loopingSoundSource)
                {
                    game->getSoundEngine()->stopAllSoundsOfSoundSource(loopingSoundSource);
                }
                isLoopingSoundPlaying = false;
                timeSinceLastSound = 0.0f;
            }
        }

        //key item collision
        if(player.collField(key.getPosition()) && kCollected == false)
        {

                kCollected = true;
                entities.pop_back();
                game->getSoundEngine()->play2D("beep.wav", false);
                std::cout << "Green Key Acquired!" << std::endl;
        }

        for (int i = 0; i < ammo.size(); i++) {
            if (player.collField(ammo[i]->getPosition()) && ammo[i]->getExist())
            {

                ammo[i]->setExist(false);
              //  entities.pop_back();
                game->getSoundEngine()->play2D("beep.wav", false);
                std::cout << "Ammo Collected!" << std::endl;
                player.addAmmo(3);
            }
        }

        for (int i = 0; i < health.size(); i++) {
            if (player.collField(health[i]->getPosition()) && health[i]->getExist())
            {
                health[i]->setExist(false);
               
              //  entities.pop_back();
                game->getSoundEngine()->play2D("beep.wav", false);
                std::cout << "Health Acquired!" << std::endl;
                player.addHealth(1);
            }
        }
        //Enemy attack check

        for (int i = 0; i < skulker.size(); i++) {
            if (player.collField(skulker[i]->getPosition())) {
                if(player.delayActive == false)
                {
                    hit = true;
                }
                player.subHealth(1);
                
                // cout << "player hit!" << endl;

            }
        }

        for (int i = 0; i < predator.size(); i++) {
            if (player.collField(predator[i]->getPosition())) {

                player.subHealth(1);
                // cout << "player hit!" << endl;

            }
        }
        for (int i = 0; i < stalker.size(); i++) {
            if (player.collField(stalker[i]->getPosition())) {

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

float Scene1::dist(float ax, float ay, float bx, float by)
{
    return(sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

// Custom comparison function to sort by shortest distance to a target position.
bool Scene1::sortByDistance(Entity* entity1, Entity* entity2) 
{
    float distanceA = dist(entity1->getPosition().x, entity1->getPosition().y, player.getPosition().x, player.getPosition().y);
    float distanceB = dist(entity2->getPosition().x, entity2->getPosition().y, player.getPosition().x, player.getPosition().y);
    return distanceA > distanceB;
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
    //zone points used for the visual area detection of enemies
    Vec2 zonePoint1;
    Vec2 zonePoint2;

    ra = -player.getOrientation() - DegToRad * 30;

    if(ra < 0)
    {
        ra += 2 * PI;
    }
    if(ra > 2*PI)
    {
        ra -= 2 * PI;
    }
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*for (int i = 0; i < predator.size(); i++) {

        EnemyMoveUpate(predator[i]);
    }

    for (int i = 0; i < skulker.size(); i++) {
        EnemyMoveUpate(skulker[i]);
    }*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

    //480 rays for every x value in the 480x320 screen
    for (r = 0; r < 480; r++)
    {
        disT = 10000000;
        // Horizontal Line Check
        dof = 0;
        float disH = 100000000;
        float hX = player.getPosition().x;
        float hY = player.getPosition().y;
        float aTan = -1 / tan(ra);
        if (ra > PI)
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
        if (ra == 0 || ra == PI)
        {
            rx = player.getPosition().x;
            ry = player.getPosition().y;
            dof = 16;
        }
        while (dof < 16)
        {
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapWallsX + mx;
            if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] > 0)
            {
                hX = rx;
                hY = ry;
                disH = dist(player.getPosition().x, player.getPosition().y, hX, hY);
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
        if (ra > P2 && ra < P3)
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
        if (ra == 0 || ra == PI)
        {
            rx = player.getPosition().x;
            ry = player.getPosition().y;
            dof = 16;
        }
        while (dof < 16)
        {
            mx = rx / 64;
            my = ry / 64;
            mp = my * mapWallsX + mx;
            if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] > 0)
            {
                vX = rx;
                vY = ry;
                disV = dist(player.getPosition().x, player.getPosition().y, vX, vY);
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
        if (disV < disH)
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
            else if (mp > 0 && mp < mapWallsX * mapWallsY && mapWalls[mp] == 5)
            {
                tempTex = textureDoor2;
            }
            //SDL_SetRenderDrawColor(renderer,color.x, color.y, color.z, 0);
            SDL_SetTextureColorMod(tempTex, color.x, color.y, color.z);
        }
        else if (disH < disV)
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
            SDL_SetTextureColorMod(tempTex, color.x / 1.6, color.y / 1.6, color.z / 1.6);
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
        SDL_Rect rect = { r, lineO, 1, lineH };

        //where textures go
        int texVX = (ry / 64 - my) * (64);
        if ((ra * RADIANS_TO_DEGREES > 90) && (ra * RADIANS_TO_DEGREES < 270))
        {
            texVX = 63 - texVX;
        }

        int texHX = (rx / 64 - mx) * (64);
        if (ra * RADIANS_TO_DEGREES < 180)
        {
            texHX = 63 - texHX;
        }

        float dy = lineO + lineH;

        if (disV < disH) //check if vertical or horizontal
        {
            SDL_Rect crop = { texVX, 0, 1, 64 }; // if vertical we use y offset
            SDL_RenderCopy(renderer, tempTex, &crop, &rect);

        }
        else
        {
            SDL_Rect crop = { texHX, 0, 1, 64 }; // if horizontal we use x offset
            SDL_RenderCopy(renderer, tempTex, &crop, &rect);
        }

        //SDL_Rect ceiling = { r, 0 , 1, lineO }; //under the walls
        //SDL_SetRenderDrawColor(renderer, 7, 11, 27, 0);
        //SDL_RenderFillRect(renderer, &ceiling);

        //SDL_RenderFillRect(renderer, &rect);
        //SDL_RenderDrawLine(renderer, r*8 + 530, lineO, r*8 + 530, lineH + lineO);

        //degree change 60 (fov)  / 480 (number of rays) = 1/8 degree change per ray
        ra += DegToRad / 8;
        if (ra < 0)
        {
            ra += 2 * PI;
        }
        if (ra > 2 * PI)
        {
            ra -= 2 * PI;
        }

        if (r == 30) {
            zonePoint1 = Vec2(rx, ry);

        }
        if (r == 450) {
            zonePoint2 = Vec2(rx, ry);

        }
    }

   
    
}



Uint32 Scene1::getpixel(SDL_Surface* surface, int x, int y)
{
    //https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    Uint32 pixelColor = *(Uint32*)p;

    Uint8 red, green, blue, alpha;
    SDL_GetRGBA(pixelColor, surface->format, &red, &green, &blue, &alpha);
    return SDL_MapRGBA(surface->format, blue, green, red, alpha);
}

void Scene1::drawFloors()
{
    //we need to start from left most fov ray, till right most fov ray
    float angle0 = -player.getOrientation() - DegToRad * 30;
    float angle1 = angle0 + DegToRad * 60;

    float rayDirX0 = cos(angle0);
    float rayDirY0 = sin(angle0);
    float rayDirX1 = cos(angle1);
    float rayDirY1 = sin(angle1);

    float posZ = 1 * 320 / 2;
    //find right most ray
    for (int y = 160; y < 320; y++) //half screen line by line
    {
        int p = y - 320 / 2;
        float rowDistance = posZ / p;

        float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / 480;
        float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / 480;

        float floorX = player.getPosition().x / 64.0f + rowDistance * rayDirX0; //posX
        float floorY = player.getPosition().y / 64.0f + rowDistance * rayDirY0; //posY

        for (int x = 0; x < 480; x++)
        {
            //point on the map
            int mapX = (int)(floorX);
            int mapY = (int)(floorY);

            //get tex coord from 0.0 - 1.0 offset
            int tx = (int)(64 * (floorX - mapX)) & (64 - 1); //64 = texWidth
            int ty = (int)(64 * (floorY - mapY)) & (64 - 1); //64 = texHeight

            floorX += floorStepX;
            floorY += floorStepY;

            //choose text and draw pixel
            if (mapFloor[mapY * mapFloorX + mapX] == 0)
            {
                //
            }
            else if (mapFloor[mapY * mapFloorX + mapX] == 1)
            {
                //
            }

            //floor
            pixels[x + y * 480] = getpixel(imageFloor, tx, ty);
            //ceiling
            pixels[x + ((320 - y + 1) * 480)] = getpixel(imageCeiling, tx, ty);

        }
    }
}

void Scene1::entityTick(Entity* entity, SDL_Texture* entityTexture)
{
    //Major help from:
    //https://www.youtube.com/watch?v=eBFOjriHMc8
    //https://wynnliam.github.io/raycaster/news/tutorial/2019/04/03/raycaster-part-02.html

    //first find distance between key and player
    int distPosX = (entity->getPosition().x) - player.getPosition().x; //entity pos x - player pos.x
    int distPosY = (entity->getPosition().y) - player.getPosition().y; //entity pos y - player pos.y

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

    //we split up the item sprite so that we can compare the distance of each slice to the distance of each wall
       //if wall is closer, we don't render that part of the sprite!
    int drawStartX = -width / 2 + xTemp;
    if (drawStartX < 0) drawStartX = 0;
    int drawEndX = width / 2 + xTemp;
    if (drawEndX >= 480) drawEndX = 480;
    for (int i = drawStartX; i <= drawEndX; i++)
    {
        if (i >= 0 && i <= 1010)
        {
            int rayPos = (int)(((i) / 480.0) * 480); //530 is where the screen starts at the moment
            //if direction of this hits wall of distance greater than sprite
            if (dist < zBuffer[rayPos]) {
                SDL_Rect spriteRect = { i, 320 / 2 + 4, 1,width };
                SDL_Rect crop = { int(i - (-width / 2 + xTemp)) * 64 / width,0,  1,  64 };
                SDL_RenderCopy(renderer, entityTexture, &crop, &spriteRect);
            }
        }

    }

}



