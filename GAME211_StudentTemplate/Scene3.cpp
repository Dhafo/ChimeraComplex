#include "Scene3.h"
#include <VMath.h>
#include <iostream>
#include <algorithm>

using namespace std;
float Scene3::PointDirection(float x1, float y1, float x2, float y2)
{
    return atan2(x2 - x1, y2 - y1) * RADIANS_TO_DEGREES;
}

// See notes about this constructor in Scene3.h.
Scene3::Scene3(SDL_Window* sdlWindow_, GameManager* game_)
{
    xAxis = 1024.0f;
    yAxis = 512.0f;
    window = sdlWindow_;
    game = game_;
    renderer = SDL_GetRenderer(window);
    float orientation = 90 * DEGREES_TO_RADIANS;
    player = Player(10, 7, orientation, Vec2(480, 928), Vec2(cos(orientation) * 1.75, -sin(orientation) * 1.75));
}

Scene3::~Scene3()
{


}

bool Scene3::OnCreate()
{
        // Initialize SDL's joystick subsystem
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    // Open the first joystick
    SDL_GameController* gameController = SDL_GameControllerOpen(0);
    //setting timers to a value
    timeSecond = 0;
    time = 0;
    ammoTimer, healthTimer, skulker1Timer, skulker2Timer = 15;
    
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    //render in 2x scale to stretch image
    SDL_RenderSetScale(renderer, 2, 2);
    //create a surface of the entire screen, store the pixels in a buffer, and create a texture from it
    screenSurface = SDL_CreateRGBSurface(0, 480, 320, 32, 0, 0, 0, 0);
    pixels = (Uint32*)screenSurface->pixels;
    buffer = SDL_CreateTextureFromSurface(renderer, screenSurface);

    //if key has been collected


    Matrix4 ndc = MMath::viewportNDC(w, h);
    Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
    projectionMatrix = ndc * ortho;

    /// Turn on the SDL imaging subsystem
    IMG_Init(IMG_INIT_PNG);

    //load all the images
    //for level generation:
    imageWall = IMG_Load("Sprites/Level/wallTest.png");
    imageWall2 = IMG_Load("Sprites/Level/wallTest2.png");
    imageDoor = IMG_Load("Sprites/Level/door.png");
    imageDoor2 = IMG_Load("Sprites/Level/door2.png");
    imageFloor = IMG_Load("Sprites/Level/floor2.png");
    imageFloor2 = IMG_Load("Sprites/Level/floor1.png");
    imageCeiling = IMG_Load("Sprites/Level/ceiling.png");
    imageCeiling2 = IMG_Load("Sprites/Level/ceiling2.png");
    //for item pickups:
    healthSprite = IMG_Load("Sprites/Entities/MedicalKit.png");
    ammoSprite = IMG_Load("Sprites/Entities/HandGunMagazine.png");
    //for enemies:
    skulkerSprite = IMG_Load("Sprites/Entities/Enemy_Skulker.png");
    bossSprite = IMG_Load("Sprites/Entities/Boss.png");
    tendrilSprite = IMG_Load("Sprites/Entities/GroundTendrils.png");

    //create textures from the images
    textureWall = SDL_CreateTextureFromSurface(renderer, imageWall);
    textureWall2 = SDL_CreateTextureFromSurface(renderer, imageWall2);
    textureDoor = SDL_CreateTextureFromSurface(renderer, imageDoor);
    textureDoor2 = SDL_CreateTextureFromSurface(renderer, imageDoor2);
    textureFloor = SDL_CreateTextureFromSurface(renderer, imageFloor);
    textureFloor2 = SDL_CreateTextureFromSurface(renderer, imageFloor2);
    textureCeiling = SDL_CreateTextureFromSurface(renderer, imageCeiling);
    textureCeiling2 = SDL_CreateTextureFromSurface(renderer, imageCeiling2);

    ammoTexture = SDL_CreateTextureFromSurface(renderer, ammoSprite);
    healthTexture = SDL_CreateTextureFromSurface(renderer, healthSprite);

    skulkerTexture = SDL_CreateTextureFromSurface(renderer, skulkerSprite);
    bossTexture = SDL_CreateTextureFromSurface(renderer, bossSprite);
    tendrilTexture = SDL_CreateTextureFromSurface(renderer, tendrilSprite);

    //load all frames of the gun animation
    for (int i = 0; i < 6; i++)
    {
        std::string gunFrame = "Sprites/Gun/gun" + std::to_string(i) + ".png";
        SDL_Surface* gunSurf = IMG_Load(gunFrame.c_str());
        SDL_Texture* gunTexture = SDL_CreateTextureFromSurface(renderer, gunSurf);
        SDL_FreeSurface(gunSurf);
        textureGun[i] = gunTexture;
    }
    currentGunFrame = 0;

    //Items
   

    // create all entities
    ammo.push_back(new Entity(Vec2(736, 928), Vec2(0, 0), ammoTexture));
   

    health.push_back(new Entity(Vec2(224, 928), Vec2(0, 0), healthTexture));

    skulker.push_back(new Enemy(3, Vec2(416, 608), Vec2(0, 0), skulkerTexture, 1.0f));
    skulker.push_back(new Enemy(3, Vec2(544, 608), Vec2(0, 0), skulkerTexture, 1.0f));
    for (int i = 0; i < skulker.size(); i++)
    {
        skulkADelay[i] = 1;
    }
  
    boss.push_back(new Enemy(15, Vec2(480, 608), Vec2(0, 0), bossTexture, 0.0f));

    //Forward attacking tendrals seperated by their grouping
    //1
    forwardTendrils.push_back(new Enemy(3, Vec2(480, 672), Vec2(0, 0), tendrilTexture, 0.0f));
    //2
    forwardTendrils.push_back(new Enemy(3, Vec2(480, 736), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(416, 736), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(544, 736), Vec2(0, 0), tendrilTexture, 0.0f));
    //3
    forwardTendrils.push_back(new Enemy(3, Vec2(480, 800), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(416, 800), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(544, 800), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(608, 800), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(352, 800), Vec2(0, 0), tendrilTexture, 0.0f));
    //4
    forwardTendrils.push_back(new Enemy(3, Vec2(480, 864), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(416, 864), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(544, 864), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(608, 864), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(352, 864), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(288, 864), Vec2(0, 0), tendrilTexture, 0.0f));
    forwardTendrils.push_back(new Enemy(3, Vec2(672, 864), Vec2(0, 0), tendrilTexture, 0.0f));

    //Side attacking tendrals seperated by their grouping
    //1
    sideTendrils.push_back(new Enemy(3, Vec2(416, 608), Vec2(0, 0), tendrilTexture, 0.0f));

    sideTendrils.push_back(new Enemy(3, Vec2(544, 608), Vec2(0, 0), tendrilTexture, 0.0f));
    //2
    sideTendrils.push_back(new Enemy(3, Vec2(352, 608), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(352, 672), Vec2(0, 0), tendrilTexture, 0.0f));

    sideTendrils.push_back(new Enemy(3, Vec2(608, 608), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(608, 672), Vec2(0, 0), tendrilTexture, 0.0f));
    //3
    sideTendrils.push_back(new Enemy(3, Vec2(288, 608), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(288, 672), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(288, 736), Vec2(0, 0), tendrilTexture, 0.0f));

    sideTendrils.push_back(new Enemy(3, Vec2(672, 608), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(672, 672), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(672, 736), Vec2(0, 0), tendrilTexture, 0.0f));
    //4
    sideTendrils.push_back(new Enemy(3, Vec2(224, 608), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(224, 672), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(224, 736), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(224, 800), Vec2(0, 0), tendrilTexture, 0.0f));

    sideTendrils.push_back(new Enemy(3, Vec2(736, 608), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(736, 672), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(736, 736), Vec2(0, 0), tendrilTexture, 0.0f));
    sideTendrils.push_back(new Enemy(3, Vec2(736, 800), Vec2(0, 0), tendrilTexture, 0.0f));

    for (int i = 1; i < forwardTendrils.size(); i++) {
        forwardTendrils[i]->setExist(false);
    }

    for (int i = 2; i < sideTendrils.size(); i++) {
        sideTendrils[i]->setExist(false);
    }


    //takes all the childs of entities and stores them and in a array to be sorted in the future
    entities.reserve( skulker.size() + ammo.size() + health.size() + forwardTendrils.size()+ sideTendrils.size() + boss.size());

  
    for (Entity* entity : skulker)
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
    for (Entity* entity : forwardTendrils)
    {
        entities.push_back(entity);
    }
    for (Entity* entity : sideTendrils)
    {
        entities.push_back(entity);
    }
    for (Entity* entity : boss)
    {
        entities.push_back(entity);
    }
  

    //for UI
    colorFont = { 238, 233, 138 };
    font = TTF_OpenFont("UI/Roboto-Regular.ttf", 24);
    healthName = TTF_RenderText_Solid(font, "Health: ", colorFont);
    healthNameTexture = SDL_CreateTextureFromSurface(renderer, healthName);
    healthNameRect = { screenOffsetX,328 + screenOffsetY,healthName->w,healthName->h };
    ammoName = TTF_RenderText_Solid(font, "Ammo: ", colorFont);
    ammoNameTexture = SDL_CreateTextureFromSurface(renderer, ammoName);
    ammoNameRect = { 320 + screenOffsetX + 64, 328 + screenOffsetY, ammoName->w,ammoName->h };

    return true;
}

void Scene3::OnDestroy()
{
    TTF_CloseFont(font);
    SDL_DestroyTexture(textureWall);
    SDL_DestroyTexture(textureWall2);
    SDL_DestroyTexture(textureFloor);
    SDL_DestroyTexture(textureFloor2);
    SDL_DestroyTexture(textureCeiling);
    SDL_DestroyTexture(textureCeiling2);
    SDL_DestroyTexture(textureDoor);
    SDL_DestroyTexture(textureDoor2);

    SDL_FreeSurface(imageWall);
    SDL_FreeSurface(imageWall2);
    SDL_FreeSurface(imageFloor);
    SDL_FreeSurface(imageFloor2);
    SDL_FreeSurface(imageCeiling);
    SDL_FreeSurface(imageCeiling2);
    SDL_FreeSurface(imageDoor);
    SDL_FreeSurface(imageDoor2);
  
    SDL_DestroyTexture(ammoTexture);
    SDL_DestroyTexture(healthTexture);
    SDL_DestroyTexture(tendrilTexture);
    SDL_DestroyTexture(bossTexture);

    SDL_FreeSurface(healthSprite);
    SDL_FreeSurface(ammoSprite);
    SDL_FreeSurface(bossSprite);
    SDL_FreeSurface(tendrilSprite);

    SDL_DestroyTexture(skulkerTexture);
    
    SDL_FreeSurface(skulkerSprite);

    SDL_DestroyTexture(ammoNameTexture);
    SDL_DestroyTexture(healthNameTexture);

    SDL_FreeSurface(ammoName);
    SDL_FreeSurface(healthName);
    for (int i = 0; i < 6; i++)
    {

        SDL_DestroyTexture(textureGun[i]);
    }
    entities.clear();

    SDL_DestroyTexture(buffer);
    SDL_FreeSurface(screenSurface);
}

static int yrel = 0;
static int xrel = 0;

void Scene3::Update(const float deltaTime)
{
    //sets scene on player death
    if (player.getCurrentHealth() <= 0)
    {
        game->LoadScene(0);
        return;
    }


    Vec3 dir = Vec3(xrel, yrel, 0.0f);
    float mag = VMath::mag(dir);

    if (mag > 8000.0f)
    {

        float angle = PointDirection(0, 0, (float)xrel, -(float)yrel) - player.getOrientation() * RADIANS_TO_DEGREES;
        float moveDistance = 0.000001f;

        float xDist = cos(angle * DEGREES_TO_RADIANS) * RADIANS_TO_DEGREES * moveDistance * mag;
        float yDist = sin(angle * DEGREES_TO_RADIANS) * RADIANS_TO_DEGREES * moveDistance * mag;

        int xSign = 0;
        if (abs(xDist) > 0.0f)
        {
            xSign = xDist / abs(xDist);
        }
        int ySign = 0;
        if (abs(yDist) > 0.0f)
        {
            ySign = yDist / abs(yDist);
        }

        float collisionDist = 14.0f;

        int gxd = floor((player.getPosition().x + xDist + collisionDist * xSign) / 64.0f);
        int gyd = floor((player.getPosition().y + yDist + collisionDist * ySign) / 64.0f);

        if (mapWalls[gyd * mapWallsX + gxd] == 0)
        {
            player.Move(xDist, yDist);
        }
        else
        {
            /// Angle sweep
            for (int a = 0; a <= 75; a += 5)
            {
                float _ang = angle + a;
                float xDist = cos(_ang * DEGREES_TO_RADIANS) * RADIANS_TO_DEGREES * moveDistance * mag;
                float yDist = sin(_ang * DEGREES_TO_RADIANS) * RADIANS_TO_DEGREES * moveDistance * mag;
                int gxd = floor((player.getPosition().x + xDist + collisionDist * xSign) / 64.0f);
                int gyd = floor((player.getPosition().y + yDist + collisionDist * ySign) / 64.0f);
                if (mapWalls[gyd * mapWallsX + gxd] == 0)
                {
                    player.Move(xDist, yDist);
                    break;
                }

                _ang = angle - a;
                xDist = cos(_ang * DEGREES_TO_RADIANS) * RADIANS_TO_DEGREES * moveDistance * mag;
                yDist = sin(_ang * DEGREES_TO_RADIANS) * RADIANS_TO_DEGREES * moveDistance * mag;
                gxd = floor((player.getPosition().x + xDist + collisionDist * xSign) / 64.0f);
                gyd = floor((player.getPosition().y + yDist + collisionDist * ySign) / 64.0f);
                if (mapWalls[gyd * mapWallsX + gxd] == 0)
                {
                    player.Move(xDist, yDist);
                    break;
                }
            }
        }

    }



    if (boss[0]->getHealth() <= 0)
    {
        boss[0]->setExist(false);
        game->LoadScene(4);
        return;
    }
    timeSecond += deltaTime;

    if (timeSecond > 1) {
        time += 1;
        timeSecond = 0;
    }
    TendrilTimeing(time);

    //if the enemies are dead their exist bools are set as false(this bool is a requirment for the object functions and rendering)

    for (int i = 0; i < skulker.size(); i++)
    {
        if (skulker[i]->getExist() && skulker[i]->getHealth() <= 0)
        {
            game->getSoundEngine()->play2D("Audio/skulkerDeath.ogg", false);
            skulker[i]->setExist(false);
        }
        if (skulkADelay[i] >= 0) {
            skulkADelay[i] -= deltaTime;
        }
    }


    //if player shoots, play animation by going through all the frames
    if (shootGun)
    {
        timePassedGun += deltaTime;
        //shoot gun
        if (timePassedGun >= 0.08f)
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

    //if player gets hit, start hit effect by changing the alpha (int fade) of a red rect over time
    if (hit)
    {
        timePassedHit += deltaTime;
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
        if (fade < 0 && fadeDir == -1)
        {
            fade = 0;
            fadeDir = 1;
            hit = false;
        }
    }

    //if player hits something
    if (aim)
    {
        timePassedHit += deltaTime;
        if (timePassedHit >= 0.1f)
        {
            game->getSoundEngine()->play2D("Audio/hit.ogg", false);
            aim = false;
            timePassedHit = 0;
        }
    }

    HandleMovement();
    player.playerUpdate(deltaTime);

    //footstep sounds
    if (player.w || player.s)
    {
        timePassedStep += deltaTime;
        if (timePassedStep >= 0.45f)
        {
            //sound from: https://freesound.org/people/swuing/sounds/38873/
            ISound* step = game->getSoundEngine()->play2D("Audio/step.wav", false, false, true);
            //generate a random number to use as a pitch to create variation in the footsteps
            float random = 0.85f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.89f - 0.85f));
            step->setPlaybackSpeed(random);
            timePassedStep = 0;
        }
    }

    //update the health/ammo values that will be shown on the UI
    _itoa_s(player.getCurrentHealth(), playerHealth, sizeof(playerHealth), 10); // Gets the health for health ttf
    _itoa_s(player.getAmmo(), playerAmmo, sizeof(playerAmmo), 10); // Gets the ammo for ammo ttf

    //depending on vision and open line of sight enemies will move towards the player




    for (int i = 0; i < skulker.size(); i++)
    {
        if (EnemyCanSeePlayer(skulker[i]) && skulkADelay[i] <= 0)
        {
            if (skulker[i]->getExist() && skulker[i]->agitated == false)
            {
                skulker[i]->agitated = true;
                game->getSoundEngine()->play2D("Audio/skulker.wav", false);
            }
            skulker[i]->updatePos(player.getPosition());
        }
        else
        {
            skulker[i]->agitated = false;
        }

    }

    Spawns(deltaTime);
        
     
}



void Scene3::Render()
{
    SDL_RenderClear(renderer);

    //take the pixel buffer from drawFloorCeiling() and update the texture with it, then render on screen
    drawFloorCeiling();
    SDL_UpdateTexture(buffer, NULL, screenSurface->pixels, screenSurface->pitch);
    SDL_RenderCopy(renderer, buffer, NULL, &gameScreen);

    //draw walls on top of this
    draw3D();

    //sort all the entities in the level, with the closest one appearing last
    std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b)
        {
            return sortByDistance(a, b);
        });

    //now that the array is sorted, we can render them in order, if they still exist
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->getExist() == true) {
            entityTick(entities[i], entities[i]->texture);
        }
    }

    //then render the gun/anim on top of everything else
    SDL_RenderCopy(renderer, textureGun[currentGunFrame], NULL, &gun);

    //for the player hit/damaged effect
    SDL_SetRenderDrawColor(renderer, 180, 15, 15, fade);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &dmgFade);

    SDL_SetRenderDrawColor(renderer, 41, 21, 70, 255);
    SDL_RenderFillRect(renderer, &Top);
    SDL_RenderFillRect(renderer, &Bottom);
    SDL_RenderFillRect(renderer, &Left);
    SDL_RenderFillRect(renderer, &Right);

    //if we collected the key, render key UI
    

    //health ui
    healthText = TTF_RenderText_Solid(font, playerHealth, colorFont);
    healthTextTexture = SDL_CreateTextureFromSurface(renderer, healthText);

    //ammo ui
    ammoText = TTF_RenderText_Solid(font, playerAmmo, colorFont);
    ammoTextTexture = SDL_CreateTextureFromSurface(renderer, ammoText);

    //update the rect for these new values
    healthRect = { screenOffsetX + healthNameRect.w,328 + screenOffsetY,healthText->w,healthText->h };
    ammoRect = { 320 + 80 + ammoNameRect.w, 328 + screenOffsetY, ammoText->w, ammoText->h };

    //render the health and ammo of the UI
    SDL_RenderCopy(renderer, healthTextTexture, NULL, &healthRect);
    SDL_RenderCopy(renderer, healthNameTexture, NULL, &healthNameRect);
    SDL_RenderCopy(renderer, ammoTextTexture, NULL, &ammoRect);
    SDL_RenderCopy(renderer, ammoNameTexture, NULL, &ammoNameRect);

    SDL_RenderPresent(renderer);

    //fix crashes thanks hassan
    SDL_DestroyTexture(healthTextTexture);
    SDL_FreeSurface(healthText);

    SDL_DestroyTexture(ammoTextTexture);
    SDL_FreeSurface(ammoText);
}

void Scene3::Shoot()
{
    //player will shoot only if they have ammo
    if (player.getAmmo() > 0)
    {
        //checks angle towards enemy and wall obstructions for the ability to damage foes

        for (int i = 0; i < skulker.size(); i++)
        {
            if (TendrilBlock(skulker[i]))
            {
                aim = true;
            }

            if (player.getDistance(skulker[i]->getPosition()) < 64) {

                if (skulker[i]->VisionCheck(player, 20) && EnemyCanSeePlayer(skulker[i]) && TendrilBlock(skulker[i]) == false)
                {
                    aim = true;
                    skulker[i]->subtractHealth(1);
                }

            }
            else {
                if (skulker[i]->VisionCheck(player, 3) && EnemyCanSeePlayer(skulker[i]) && TendrilBlock(skulker[i]) == false)
                {
                    aim = true;
                    skulker[i]->subtractHealth(1);
                }
            }

        }

        if (TendrilBlock(boss[0]))
        {
            aim = true;
        }
        if (player.getDistance(boss[0]->getPosition()) < 64) {

            if (boss[0]->getExist() && boss[0]->VisionCheck(player, 20) && EnemyCanSeePlayer(boss[0]) && TendrilBlock(boss[0]) == false)
            {
                aim = true;
                boss[0]->subtractHealth(1);
            }

        }
        else {
            if (boss[0]->getExist() && boss[0]->VisionCheck(player, 3) && EnemyCanSeePlayer(boss[0]) && TendrilBlock(boss[0]) == false)
            {
                aim = true;
                boss[0]->subtractHealth(1);
            }
        }



        game->getSoundEngine()->play2D("Audio/pistol_shot.wav", false);
        shootGun = true;
        player.subAmmo(1);
    }
}

static int cam = 0;

void Scene3::HandleEvents(const SDL_Event& event)
{
    if (event.type == SDL_CONTROLLERAXISMOTION)
    {
        // Handle move
        if (event.jaxis.which == 0)
        {
            if (event.jaxis.axis == 0)
            {
                xrel = event.jaxis.value;
            }

            else if (event.jaxis.axis == 1) // Y-axis motion
            {
                yrel = event.jaxis.value;
            }

            else if (event.jaxis.axis == 2)
            {
                cam = event.jaxis.value;
            }

            else if (event.jaxis.axis == 5) //trigger right?
            {
                if (event.jaxis.value > 8000 && !shoot)
                {
                    shoot = true;
                    Shoot();
                }
                else if (event.jaxis.value <= 0)
                {
                    shoot = false;
                }
            }
        }
    }
    else if (event.type == SDL_CONTROLLERBUTTONDOWN)
    {
        if (event.cbutton.which == 0) // Check if the event is from the first joystick
        {
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) // Check if it's the right trigger
            {
                Shoot();
            }
            else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
            {
                Interact();
            }

            else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                Interact();
            }
        }
    }
    else if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_LCTRL)
        {
            Shoot();
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
        {
            player.a = 1;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
        {
            player.d = 1;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_UP)
        {
            player.w = 1;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
        {
            player.s = 1;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            Interact();
        }
    }
    if (event.type == SDL_KEYUP)
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

void Scene3::Interact()
{
    //check if interactable is close enough in front of us
    int xOffset = 0, yOffset = 0;
    if (player.getVelocity().x < 0)
    {
        xOffset = -25;
    }
    else
    {
        xOffset = 25;
    }
    if (player.getVelocity().y < 0)
    {
        yOffset = -25;
    }
    else
    {
        yOffset = 25;
    }
    //using the offsets, we shoot a ray in front of us and check what kind of interactable it is on the grid
    int gridPlayerX_add_xOffset = (player.getPosition().x + xOffset) / 64.0;
    int gridPlayerY_add_yOffset = (player.getPosition().y + yOffset) / 64.0;
    //4 = regular door
    if (mapWalls[gridPlayerY_add_yOffset * mapWallsX + gridPlayerX_add_xOffset] == 4)
    {
        game->getSoundEngine()->play2D("Audio/door.wav", false);
        mapWalls[gridPlayerY_add_yOffset * mapWallsX + gridPlayerX_add_xOffset] = 0;
    }
    //5 = door locked by green keycard

}

bool Scene3::EnemyCanSeePlayer(Enemy* enemy_)
{
    int mapX, mapY, mapIndex, dof;
    float rayX, rayY;
    float xOffset = 0;
    float yOffset = 0;

    float distance;
    float playerX = player.getPosition().x;
    float playerY = player.getPosition().y;

    float enemyX, enemyY, enemyPlayerAngle, rayAngle;

    enemyX = enemy_->getPosition().x;
    enemyY = enemy_->getPosition().y;

    enemyPlayerAngle = atan2(-(playerY - enemyY), playerX - enemyX);
    enemy_->setOrientation(enemyPlayerAngle);

    distance = 10000000;
    // Horizontal Line Check
    rayAngle = -enemy_->getOrientation();
    dof = 0;
    float disH = 100000000;
    float horiX, horiY;
    float aTan = -1 / tan(rayAngle);

    if (rayAngle < 0)
    {
        rayAngle += 2 * PI;
    }

    if (rayAngle > 2 * PI)
    {
        rayAngle -= 2 * PI;
    }

    if (rayAngle > PI)
    {
        rayY = (int)(enemy_->getPosition().y / 64) * 64 - 0.0001;
        rayX = (enemy_->getPosition().y - rayY) * aTan + enemy_->getPosition().x;
        yOffset = -64;
        xOffset = -yOffset * aTan;
    }

    if (rayAngle < PI)
    {
        rayY = (int)(enemy_->getPosition().y / 64) * 64 + 64;
        rayX = (enemy_->getPosition().y - rayY) * aTan + enemy_->getPosition().x;
        yOffset = 64;
        xOffset = -yOffset * aTan;
    }

    if (rayAngle == 0 || rayAngle == PI)
    {
        rayX = enemy_->getPosition().x;
        rayY = enemy_->getPosition().y;
        dof = 16;
    }

    while (dof < 16)
    {
        mapX = rayX / 64;
        mapY = rayY / 64;
        mapIndex = mapY * mapWallsX + mapX;
        //if we hit a wall then stop
        if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] > 0)
        {
            horiX = rayX;
            horiY = rayY;
            disH = dist(enemy_->getPosition().x, enemy_->getPosition().y, horiX, horiY);
            dof = 16;
        }
        else
        {
            rayX += xOffset;
            rayY += yOffset;
            dof += 1;
        }
    }

    // Vertical Line Check
    dof = 0;
    float disV = 100000000;
    float vertX, vertY;
    float nTan = -tan(rayAngle);
    if (rayAngle > P2 && rayAngle < P3)
    {
        rayX = (int)(enemy_->getPosition().x / 64) * 64 - 0.0001;
        rayY = (enemy_->getPosition().x - rayX) * nTan + enemy_->getPosition().y;
        xOffset = -64;
        yOffset = -xOffset * nTan;
    }
    if (rayAngle < P2 || rayAngle > P3)
    {
        rayX = (int)(enemy_->getPosition().x / 64) * 64 + 64;
        rayY = (enemy_->getPosition().x - rayX) * nTan + enemy_->getPosition().y;
        xOffset = 64;
        yOffset = -xOffset * nTan;
    }
    if (rayAngle == 0 || rayAngle == PI)
    {
        rayX = enemy_->getPosition().x;
        rayY = enemy_->getPosition().y;
        dof = 16;
    }
    while (dof < 16)
    {
        mapX = rayX / 64;
        mapY = rayY / 64;
        mapIndex = mapY * mapWallsX + mapX;
        //if we hit a wall then stop
        if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] > 0)
        {
            vertX = rayX;
            vertY = rayY;
            disV = dist(enemy_->getPosition().x, enemy_->getPosition().y, vertX, vertY);
            dof = 16;
        }
        else
        {
            rayX += xOffset;
            rayY += yOffset;
            dof += 1;
        }
    }
    if (disV < disH)
    {
        rayX = vertX;
        rayY = vertY;
        distance = disV;
        mapX = rayX / 64;
        mapY = rayY / 64;
        mapIndex = mapY * mapWallsX + mapX;
    }
    else if (disH < disV)
    {
        rayX = horiX;
        rayY = horiY;
        distance = disH;
        mapX = rayX / 64;
        mapY = rayY / 64;
        mapIndex = mapY * mapWallsX + mapX;
    }
    //if distance of wall is farther than player, then enemy can see
    if (distance > dist(enemyX, enemyY, playerX, playerY)) {
        return true;
    }
    else {
        return false;
    }
}

void Scene3::HandleMovement()
{
    //if player.a or player.d is 1, we are in the process of turning the player
    if (cam < -8000)
    {
        float normCam = 2 * (cam + 32768.0f) / (32767.0f + 32768.0f) - 1.0f;
        player.setOrientation(player.getOrientation() - 0.028 * normCam);
        if (player.getOrientation() > 2 * PI)
        {
            player.setOrientation(player.getOrientation() - 2 * PI);
        }
        player.setvelocity(Vec2(cos(-player.getOrientation()) * 1.75f, sin(-player.getOrientation()) * 1.75));
    }

    else if (cam > 8000)
    {
        float normCam = 2 * (cam + 32768.0f) / (32767.0f + 32768.0f) - 1.0f;
        player.setOrientation(player.getOrientation() - 0.028 * normCam);
        if (player.getOrientation() < 0)
        {
            player.setOrientation(player.getOrientation() + 2 * PI);
        }
        player.setvelocity(Vec2(cos(-player.getOrientation()) * 1.75f, sin(-player.getOrientation()) * 1.75));
    }
    if (player.d == 1)
    {
        player.setOrientation(player.getOrientation() - 0.028);
        if (player.getOrientation() < 0)
        {
            player.setOrientation(player.getOrientation() + 2 * PI);
        }
        player.setvelocity(Vec2(cos(-player.getOrientation()) * 1.75f, sin(-player.getOrientation()) * 1.75));
    }
    else if (player.a == 1)
    {
        player.setOrientation(player.getOrientation() + 0.028);
        if (player.getOrientation() < 0)
        {
            player.setOrientation(player.getOrientation() + 2 * PI);
        }
        player.setvelocity(Vec2(cos(-player.getOrientation()) * 1.75f, sin(-player.getOrientation()) * 1.75));
    }
    //if player.w or player.s is 1, we are moving
    //using the offsets, we calculate what the new player position will be, as long as the new position isn't a wall
    int xOffset = 0, yOffset = 0;
    if (player.getVelocity().x < 0)
    {
        xOffset = -20;
    }
    else
    {
        xOffset = 20;
    }
    if (player.getVelocity().y < 0)
    {
        yOffset = -20;
    }
    else
    {
        yOffset = 20;
    }
    int gridPlayerX = player.getPosition().x / 64.0, gridPlayerX_add_xOffset = (player.getPosition().x + xOffset) / 64.0, gridPlayerX_sub_xOffset = (player.getPosition().x - xOffset) / 64.0;
    int gridPlayerY = player.getPosition().y / 64.0, gridPlayerY_add_yOffset = (player.getPosition().y + yOffset) / 64.0, gridPlayerY_sub_yOffset = (player.getPosition().y - yOffset) / 64.0;
    if (player.w == 1)
    {
        if (mapWalls[gridPlayerY * mapWallsX + gridPlayerX_add_xOffset] == 0)
        {

            player.setPosition(Vec2(player.getPosition().x + player.getVelocity().x, player.getPosition().y));
        }
        if (mapWalls[gridPlayerY_add_yOffset * mapWallsX + gridPlayerX] == 0)
        {
            player.setPosition(Vec2(player.getPosition().x, player.getPosition().y + player.getVelocity().y));
        }
    }

    if (player.s == 1)
    {
        if (mapWalls[gridPlayerY * mapWallsX + gridPlayerX_sub_xOffset] == 0)
        {
            player.setPosition(Vec2(player.getPosition().x - player.getVelocity().x, player.getPosition().y));

        }
        if (mapWalls[gridPlayerY_sub_yOffset * mapWallsX + gridPlayerX] == 0)
        {
            player.setPosition(Vec2(player.getPosition().x, player.getPosition().y - player.getVelocity().y));
        }
    }

    //after moving, we go through each of the possible collisions:
    //starting with picking up the key
   
    //picking up the ammo
    for (int i = 0; i < ammo.size(); i++) {
        if (player.collField(ammo[i]->getPosition()) && ammo[i]->getExist())
        {

            ammo[i]->setExist(false);
        attribution: https://freesound.org/people/zivs/sounds/433771/
            game->getSoundEngine()->play2D("Audio/ammo.ogg", false);
            std::cout << "Ammo Collected!" << std::endl;
            player.addAmmo(7);
        }
    }
    //picking up the health
    for (int i = 0; i < health.size(); i++) {
        if (player.collField(health[i]->getPosition()) && health[i]->getExist())
        {
            health[i]->setExist(false);


            game->getSoundEngine()->play2D("Audio/beep.wav", false);
            std::cout << "Health Acquired!" << std::endl;
            player.addHealth(5);
        }
    }

    //getting hit by an enemy
    for (int i = 0; i < skulker.size(); i++) {
        if (skulker[i]->getExist() && player.collField(skulker[i]->getPosition()))
        {
            if (player.delayActive == false)
            {
                game->getSoundEngine()->play2D("Audio/pain.wav", false);
                hit = true;
            }
            player.subHealth(2);
            skulkADelay[i] = 1;
        }
    }
    for (int i = 0; i < forwardTendrils.size(); i++) {
        if (forwardTendrils[i]->getExist() && player.collField(forwardTendrils[i]->getPosition()))
        {
            if (player.delayActive == false)
            {
                game->getSoundEngine()->play2D("Audio/pain.wav", false);
                hit = true;
            }
            player.subHealth(3);
        }
    }
    for (int i = 0; i < sideTendrils.size(); i++) {
        if (sideTendrils[i]->getExist() && player.collField(sideTendrils[i]->getPosition()))
        {
            if (player.delayActive == false)
            {
                game->getSoundEngine()->play2D("Audio/pain.wav", false);
                hit = true;
            }
            player.subHealth(3);
        }
    }

    if (boss[0]->getExist() && player.collField(boss[0]->getPosition()))
    {
        if (player.delayActive == false)
        {
            game->getSoundEngine()->play2D("Audio/pain.wav", false);
            hit = true;
        }
        player.subHealth(5);
    }
   
   

}

void Scene3::drawMap2D()
{
    //we go through the entire map array and render a 2D grid of different colored squares for each index, currently unused
    for (int y = 0; y < mapWallsY; y++)
    {
        for (int x = 0; x < mapWallsX; x++)
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
            SDL_Rect rect = { ((int)mapWallsSize * x / 2), ((int)mapWallsSize * y / 2), mapWallsSize / 2, mapWallsSize / 2 }; //divided by 2 so that map is half size but can be changed later (unused)
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

float Scene3::dist(float ax, float ay, float bx, float by)
{
    return(sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

bool Scene3::sortByDistance(Entity* entity1, Entity* entity2)
{
    float distanceA = dist(entity1->getPosition().x, entity1->getPosition().y, player.getPosition().x, player.getPosition().y);
    float distanceB = dist(entity2->getPosition().x, entity2->getPosition().y, player.getPosition().x, player.getPosition().y);
    return distanceA > distanceB;
}

void Scene3::draw3D()
{
    int mapX, mapY, mapIndex, dof;

    float rayX, rayY, rayAngle;
    float xOffset = 0;
    float yOffset = 0;

    float disT;

    SDL_Texture* tempTex = textureWall;
    rayAngle = -player.getOrientation() - DegToRad * 30;

    if (rayAngle < 0)
    {
        rayAngle += 2 * PI;
    }
    if (rayAngle > 2 * PI)
    {
        rayAngle -= 2 * PI;
    }

    //480 rays for every x value in the 480x320 screen
    for (int rayNum = 0; rayNum < 480; rayNum++)
    {
        disT = 10000000;
        // Horizontal Line Check
        dof = 0;
        float disH = 100000000;
        float horiX, horiY; //horizontal ray hit
        float aTan = -1 / tan(rayAngle);
        if (rayAngle > PI)
        {
            rayY = (int)(player.getPosition().y / 64) * 64 - 0.0001;
            rayX = (player.getPosition().y - rayY) * aTan + player.getPosition().x;
            yOffset = -64;
            xOffset = -yOffset * aTan;
        }
        if (rayAngle < PI)
        {
            rayY = (int)(player.getPosition().y / 64) * 64 + 64;
            rayX = (player.getPosition().y - rayY) * aTan + player.getPosition().x;
            yOffset = 64;
            xOffset = -yOffset * aTan;
        }
        if (rayAngle == 0 || rayAngle == PI)
        {
            rayX = player.getPosition().x;
            rayY = player.getPosition().y;
            dof = 16;
        }
        while (dof < 16)
        {
            mapX = rayX / 64;
            mapY = rayY / 64;
            mapIndex = mapY * mapWallsX + mapX;
            if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] > 0)
            {
                horiX = rayX;
                horiY = rayY;
                disH = dist(player.getPosition().x, player.getPosition().y, horiX, horiY);
                dof = 16;
            }
            else
            {
                rayX += xOffset;
                rayY += yOffset;
                dof += 1;
            }
        }

        // Vertical Line Check
        dof = 0;
        float disV = 100000000;
        float vertX, vertY;
        float nTan = -tan(rayAngle);
        if (rayAngle > P2 && rayAngle < P3)
        {
            rayX = (int)(player.getPosition().x / 64) * 64 - 0.0001;
            rayY = (player.getPosition().x - rayX) * nTan + player.getPosition().y;
            xOffset = -64;
            yOffset = -xOffset * nTan;
        }
        if (rayAngle < P2 || rayAngle > P3)
        {
            rayX = (int)(player.getPosition().x / 64) * 64 + 64;
            rayY = (player.getPosition().x - rayX) * nTan + player.getPosition().y;
            xOffset = 64;
            yOffset = -xOffset * nTan;
        }
        if (rayAngle == 0 || rayAngle == PI)
        {
            rayX = player.getPosition().x;
            rayY = player.getPosition().y;
            dof = 16;
        }
        while (dof < 16)
        {
            mapX = rayX / 64;
            mapY = rayY / 64;
            mapIndex = mapY * mapWallsX + mapX;
            if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] > 0)
            {
                vertX = rayX;
                vertY = rayY;
                disV = dist(player.getPosition().x, player.getPosition().y, vertX, vertY);
                dof = 16;
            }
            else
            {
                rayX += xOffset;
                rayY += yOffset;
                dof += 1;
            }
        }
        //see if closest collision is the vertical or horizontal line, if horizontal darken the texture
        if (disV < disH)
        {
            rayX = vertX;
            rayY = vertY;
            disT = disV;
            mapX = rayX / 64;
            mapY = rayY / 64;
            mapIndex = mapY * mapWallsX + mapX;
            //depending on index value, load different texture
            if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] == 1)
            {
                tempTex = textureWall;
            }
            else if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] == 2)
            {
                tempTex = textureWall2;
            }
            else if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] == 4)
            {
                tempTex = textureDoor;
            }
            else if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] == 5)
            {
                tempTex = textureDoor2;
            }
            SDL_SetTextureColorMod(tempTex, color.x, color.y, color.z);
        }
        else if (disH < disV)
        {
            rayX = horiX;
            rayY = horiY;
            disT = disH;
            mapX = rayX / 64;
            mapY = rayY / 64;
            mapIndex = mapY * mapWallsX + mapX;
            //depending on index value, load different texture
            if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] == 1)
            {
                tempTex = textureWall;
            }
            else if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] == 2)
            {
                tempTex = textureWall2;
            }
            else if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] == 4)
            {
                tempTex = textureDoor;
            }
            else if (mapIndex > 0 && mapIndex < mapWallsX * mapWallsY && mapWalls[mapIndex] == 5)
            {
                tempTex = textureDoor2;
            }
            SDL_SetTextureColorMod(tempTex, color.x / 1.6, color.y / 1.6, color.z / 1.6);
        }


        //3D Wall Drawing
        float correctionAngle = player.getOrientation() + rayAngle;

        if (correctionAngle < 0)
        {
            correctionAngle += 2 * PI;
        }
        if (correctionAngle > 2 * PI)
        {
            correctionAngle -= 2 * PI;
        }

        //correct fisheye
        disT *= cos(correctionAngle);

        //add the final hit distance into our zBuffer
        zBuffer[rayNum] = disT;

        float lineH = (mapWallsSize * 320) / disT; //line we draw to mimic the 3d view using the wall size, screen height and distance.
        float lineO = 160 - lineH / 2; //offset so we don't start from the very top of the screen

        //make our line into a rect
        SDL_Rect rect = { screenOffsetX + rayNum, screenOffsetY + lineO, 1, lineH };

        //where our textures go. depending on the angle, we mirror our textures or not
        int texVX = (rayY / 64 - mapY) * (64);

        if ((rayAngle * RADIANS_TO_DEGREES > 90) && (rayAngle * RADIANS_TO_DEGREES < 270))
        {
            texVX = 63 - texVX;
        }

        int texHX = (rayX / 64 - mapX) * (64);

        if (rayAngle * RADIANS_TO_DEGREES < 180)
        {
            texHX = 63 - texHX;
        }

        //check if vertical or horizontal then render
        if (disV < disH)
        {
            SDL_Rect crop = { texVX, 0, 1, 64 }; // if vertical we use y offset
            SDL_RenderCopy(renderer, tempTex, &crop, &rect);

        }
        else
        {
            SDL_Rect crop = { texHX, 0, 1, 64 }; // if horizontal we use x offset
            SDL_RenderCopy(renderer, tempTex, &crop, &rect);
        }
        //iterate to next ray by incrementing the ray angle variable
        //degree change 60 (fov)  / 480 (number of rays) = 1/8 degree change per ray
        rayAngle += DegToRad / 8;
        if (rayAngle < 0)
        {
            rayAngle += 2 * PI;
        }
        if (rayAngle > 2 * PI)
        {
            rayAngle -= 2 * PI;
        }


    }
}

Uint32 Scene3::getpixel(SDL_Surface* surface, int x, int y)
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

void Scene3::TendrilTimeing(int time)
{
    // depending on the time the different tendrils will be actived in a pattern.
    if (time < 15) {
        if (time % 3 == 0 && time % 6 != 0) {
            forwardTendrils[0]->setExist(true);
            for (int i = 1; i < forwardTendrils.size(); i++) {
                forwardTendrils[i]->setExist(false);
            }
        }
        if (time % 6 == 0) {
            forwardTendrils[0]->setExist(false);
            for (int i = 1; i < 4; i++) {
                forwardTendrils[i]->setExist(true);
            }
            //
            sideTendrils[0]->setExist(true);
            sideTendrils[1]->setExist(true);
        }
        if (time % 9 == 0) {
            for (int i = 1; i < 4; i++) {
                forwardTendrils[i]->setExist(false);
            }
            for (int i = 4; i < 9; i++) {
                forwardTendrils[i]->setExist(true);
            }
            //
            sideTendrils[0]->setExist(false);
            sideTendrils[1]->setExist(false);
            for (int i = 2; i < 6; i++) {
                sideTendrils[i]->setExist(true);
            }
        }
        if (time % 12 == 0) {

            for (int i = 9; i < 16; i++) {
                forwardTendrils[i]->setExist(true);
            }
            for (int i = 4; i < 9; i++) {
                forwardTendrils[i]->setExist(false);
            }
            //
            for (int i = 2; i < 6; i++) {
                sideTendrils[i]->setExist(false);
            }

            for (int i = 6; i < 14; i++) {
                sideTendrils[i]->setExist(false);
            }
        }
    }
    else {
        if (time % 3 == 0 && time % 6 != 0) {

            forwardTendrils[0]->setExist(true);
            for (int i = 1; i < forwardTendrils.size(); i++) {
                forwardTendrils[i]->setExist(false);
            }
            for (int i = 4; i < 9; i++) {
                forwardTendrils[i]->setExist(true);
            }
            for (int i = 9; i < 16; i++) {
                forwardTendrils[i]->setExist(false);
            }
            //
            sideTendrils[0]->setExist(false);
            sideTendrils[1]->setExist(false);
            for (int i = 2; i < 6; i++) {
                sideTendrils[i]->setExist(true);
            }
            for (int i = 6; i < 12; i++) {
                sideTendrils[i]->setExist(false);
            }
            for (int i = 12; i < 20; i++) {
                sideTendrils[i]->setExist(true);
            }
        }
        else  if (time % 6 == 0) {
            forwardTendrils[0]->setExist(false);
            for (int i = 1; i < 4; i++) {
                forwardTendrils[i]->setExist(true);
            }
            for (int i = 9; i < 16; i++) {
                forwardTendrils[i]->setExist(true);
            }
            for (int i = 4; i < 9; i++) {
                forwardTendrils[i]->setExist(false);
            }
            //
            sideTendrils[0]->setExist(true);
            sideTendrils[1]->setExist(true);
            for (int i = 2; i < 6; i++) {
                sideTendrils[i]->setExist(false);
            }
            for (int i = 6; i < 12; i++) {
                sideTendrils[i]->setExist(true);
            }
            for (int i = 12; i < 20; i++) {
                sideTendrils[i]->setExist(false);
            }
        }

    }
}

void Scene3::Spawns(float deltatime)
{
    if (health[0]->getExist() == false) {
        healthTimer -= deltatime;

        if (healthTimer <= 0) {
            health[0]->setExist(true);
            healthTimer = 15.0f;

        }
    }
    if (ammo[0]->getExist() == false) {
         ammoTimer -= deltatime;

        if (ammoTimer <= 0) {
            ammo[0]->setExist(true);
            ammoTimer = 15.0f;

        }
    }
    if (skulker[0]->getExist() == false) {
        skulker1Timer -= deltatime;

        if (skulker1Timer <= 0) {

            skulker[0]->setPosition(Vec2(416, 608));
            skulker[0]->addHealth(3);
            skulker[0]->setExist(true);
            skulker1Timer = 15.0f;

        }
    }
    if (skulker[1]->getExist() == false) {
        skulker2Timer -= deltatime;

        if (skulker2Timer <= 0) {

            skulker[1]->setPosition(Vec2(544, 608));
            skulker[1]->addHealth(3);
            skulker[1]->setExist(true);
            skulker2Timer = 15.0f;

        }
    }
}

bool Scene3::TendrilBlock(Enemy* enemy)
{
    for (int i = 0; i < forwardTendrils.size(); i++)
    {
        if (forwardTendrils[i]->getExist()&& player.isCloser( forwardTendrils[i]->getPosition(), enemy->getPosition()) &&  forwardTendrils[i]->VisionCheck(player,4))
        {

            return true;
        }

    }

    for (int i = 0; i < sideTendrils.size(); i++)
    {
        if (sideTendrils[i]->getExist() && player.isCloser(sideTendrils[i]->getPosition(), enemy->getPosition()) && sideTendrils[i]->VisionCheck(player, 4))
        {

            return true;
        }

    }
    
    return false;
}

void Scene3::drawFloorCeiling()
{
    //big help from lodev again: https://lodev.org/cgtutor/raycasting2.html
    //also this resource helped reduce lag by offering an alternative to rendercopying rects: https://benedicthenshaw.com/soft_render_sdl2.html

    //we need to start from left most fov ray, till right most fov ray
    float angleLeftMost = -player.getOrientation() - DegToRad * 30;
    float angleRightMost = angleLeftMost + DegToRad * 60;

    float rayDirXL = cos(angleLeftMost);
    float rayDirYL = sin(angleLeftMost);
    float rayDirXR = cos(angleRightMost);
    float rayDirYR = sin(angleRightMost);

    float posZ = 1 * 320 / 2;

    //find right most ray
    for (int y = 160; y < 320; y++) //half screen line by line
    {
        int p = y - 320 / 2;
        float rowDistance = posZ / p;

        float floorStepX = rowDistance * (rayDirXR - rayDirXL) / 480;
        float floorStepY = rowDistance * (rayDirYR - rayDirYL) / 480;

        float floorX = player.getPosition().x / 64.0f + rowDistance * rayDirXL; //posX
        float floorY = player.getPosition().y / 64.0f + rowDistance * rayDirYL; //posY

        for (int x = 0; x < 480; x++)
        {
            //point on the map
            int mapX = (int)(floorX);
            int mapY = (int)(floorY);

            //get tex coord from 0.0 - 1.0 offset
            int texX = (int)(64 * (floorX - mapX)) & (64 - 1); //64 = texWidth
            int texY = (int)(64 * (floorY - mapY)) & (64 - 1); //64 = texHeight

            floorX += floorStepX;
            floorY += floorStepY;

            //floor
            if (mapFloor[mapY * mapWallsX + mapX] == 0)
            {
                pixels[x + y * 480] = getpixel(imageFloor, texX, texY);
            }
            else if (mapFloor[mapY * mapWallsX + mapX] == 1)
            {
                pixels[x + y * 480] = getpixel(imageFloor2, texX, texY);
            }
            //ceiling
            if (mapCeiling[mapY * mapWallsX + mapX] == 0)
            {
                pixels[x + ((320 - y + 1) * 480)] = getpixel(imageCeiling, texX, texY);
            }
            else if (mapCeiling[mapY * mapWallsX + mapX] == 1)
            {
                pixels[x + ((320 - y + 1) * 480)] = getpixel(imageCeiling2, texX, texY);
            }
            

        }
    }
}

void Scene3::entityTick(Entity* entity, SDL_Texture* entityTexture)
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
                SDL_Rect spriteRect = { screenOffsetX + i, screenOffsetY + 320 / 2 + 4, 1,width };
                SDL_Rect crop = { int(i - (-width / 2 + xTemp)) * 64 / width,0,  1,  64 };
                SDL_RenderCopy(renderer, entityTexture, &crop, &spriteRect);
            }
        }

    }

}



