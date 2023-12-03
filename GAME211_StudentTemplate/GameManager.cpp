#include "GameManager.h"
#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene4.h"

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
    player = nullptr;
}

bool GameManager::OnCreate() {
    //initialize the ttf
    if (TTF_Init() < 0) {
        std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

        // error starting up the engine
    // My display is 1920 x 1080 but the following seems to work best to fill the screen.
    //const int SCREEN_WIDTH = 1540;
    //const int SCREEN_HEIGHT = 860;

    // Use 1000x600 for less than full screen
    const int SCREEN_WIDTH = 960;
    const int SCREEN_HEIGHT = 640;

    windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

    engine = createIrrKlangDevice();

    if (!engine)
    {
        std::cout << "error: sound engine fail" << std::endl;
        return 0;
    }

    currentScene = new Scene0(windowPtr->GetSDL_Window(), this);
    
    // create player
    float mass = 1.0f;
    float radius = 0.5f;
    float orientation = 0.0f;
    float rotation = 0.0f;
    float angular = 0.0f;
    Vec3 position(0.5f * currentScene->getxAxis() - 300, 0.5f * currentScene->getyAxis(), 0.0f);
    Vec3 velocity(cos(orientation) * 1.25, sin(orientation) * 1.25, 0.0f);
    Vec3 acceleration(0.0f, 0.0f, 0.0f);
    

    player = new PlayerBody
    (
        position,
        velocity,
        acceleration,
        mass,
        radius,
        orientation,
        rotation,
        angular,
        this
    );
    if ( player->OnCreate() == false ) {
        OnDestroy();
        return false;
    }

    // need to create Player before validating scene
    if (!ValidateCurrentScene()) {
        OnDestroy();
        return false;
    }

    // create some user defined event
    changeScenceEventType = SDL_RegisterEvents(1);
    if (changeScenceEventType == ((Uint32)-1))
    {
        OnDestroy();
        return false;

    }
    
   
	return true;
}

Uint32 GameManager::getChangeScence() {
    return changeScenceEventType;

}

/// Here's the whole game loop
void GameManager::Run() {
    
	timer->Start();
    
	while (isRunning) {
        
        handleEvents();
		timer->UpdateFrameTicks();
        if (currentScene) {
            currentScene->Update(timer->GetDeltaTime());
            currentScene->Render();
        }
		/// Keep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

void GameManager::handleEvents()
{
    SDL_Event event;

    // Let's add mouse movement and position
    // https://wiki.libsdl.org/SDL_GetMouseState

    SDL_PumpEvents();  // make sure we have the latest mouse state.

    //https://www.youtube.com/watch?v=SYrRMr4BaD4&list=PLM7LHX-clszBIGsrh7_3B2Pi74AhMpKhj&index=3

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else if (event.type == changeScenceEventType)
        {
            //switch scene
            currentScene->OnDestroy();
            delete currentScene;

            // this will take the user code and run the case
            switch (event.user.code)
            {
            case 1:
                currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
                if (!currentScene->OnCreate())
                {
                    isRunning = false;
                }
                break;

            case 2:
                isRunning = false;
                currentScene = nullptr;
                break;

            case 3:
                currentScene = new Scene4(windowPtr->GetSDL_Window(), this);
                if (!currentScene->OnCreate())
                {
                    isRunning = false;
                }
                break;

            case 4:
                currentScene = new Scene0(windowPtr->GetSDL_Window(), this);
                if (!currentScene->OnCreate())
                {
                    isRunning = false;
                }
                break;

            }


        }


        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false;
                break;
            case SDL_SCANCODE_Q:
                isRunning = false;
                break;
            case SDL_SCANCODE_DELETE:
                isRunning = false;
                break;
            case SDL_SCANCODE_1:
                LoadScene(0);
                break;
            case SDL_SCANCODE_2:
                LoadScene(1);
                break;
            case SDL_SCANCODE_3:
                LoadScene(2);
                break;

            default:
                break;
            }
        }

        if (currentScene) {
            currentScene->HandleEvents(event);
        }
    }
}



GameManager::~GameManager() {}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
    TTF_Quit();
    engine->drop();
}

// This might be unfamiliar
float GameManager::getSceneHeight() { return currentScene->getyAxis(); }

// This might be unfamiliar
float GameManager::getSceneWidth() { return currentScene->getxAxis(); }

// This might be unfamiliar
Matrix4 GameManager::getProjectionMatrix()
{ return currentScene->getProjectionMatrix(); }

// This might be unfamiliar
SDL_Renderer* GameManager::getRenderer()
{
    // [TODO] might be missing some SDL error checking
    SDL_Window* window = currentScene->getWindow();
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    return renderer;
}

PlayerBody* GameManager::setPlayer()
{
    float mass = 1.0f;
    float radius = 0.5f;
    float orientation = 0.0f;
    float rotation = 0.0f;
    float angular = 0.0f;
    Vec3 position(0.5f * currentScene->getxAxis() - 300, 0.5f * currentScene->getyAxis(), 0.0f);
    Vec3 velocity(cos(orientation) * 1.75, sin(orientation) * 1.75, 0.0f);
    Vec3 acceleration(0.0f, 0.0f, 0.0f);

    player = new PlayerBody(position,
        velocity,
        acceleration,
        mass,
        radius,
        orientation,
        rotation,
        angular,
        this
    );
    return nullptr;
}

// This might be unfamiliar
void GameManager::RenderPlayer(float scale)
{
    player->Render(scale);
}

void GameManager::LoadScene( int i )
{
    // cleanup of current scene before loading another one
    currentScene->OnDestroy();
    delete currentScene;

    switch ( i )
    {
        case 0:
            currentScene = new Scene0( windowPtr->GetSDL_Window(), this);
            engine->stopAllSounds();
            break;
        case 1:
            currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
            break;
        case 2:
            currentScene = new Scene2(windowPtr->GetSDL_Window(), this);
            break;
        default:
            currentScene = new Scene0( windowPtr->GetSDL_Window(), this);
            break;
    }

    // using ValidateCurrentScene() to safely run OnCreate
    if (!ValidateCurrentScene())
    {
        isRunning = false;
    }
}

bool GameManager::ValidateCurrentScene()
{
    if (currentScene == nullptr) {
        return false;
    }
    if (currentScene->OnCreate() == false) {
        return false;
    }
    return true;
}
