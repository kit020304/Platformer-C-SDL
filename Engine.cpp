#include "Engine.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Warrior.h"
#include "Input.h"
#include "Timer.h"
#include "MapParser.h"
#include <iostream>
#include "Camera.h"
#include "Enemy.h"
#include "Menu.h"
#include "Sound.h"


Engine* Engine::s_Instance = nullptr;
long int Engine::EndTime = 0;
long int Engine::StartTime = 0;
void StartPause()
{
    Mix_HaltChannel(2);
    Pause::GetInstance()->Run();
    while (Pause::GetInstance()->IsRunning())
    {
        Pause::GetInstance()->Events();
        Pause::GetInstance()->Update();
        Pause::GetInstance()->Render();
        Timer::GetInstance()->Tick();
    }
    Mix_Resume(2);
}
void DeathScreen(){
    Death::GetInstance()->Run();
    while(Death::GetInstance()->IsRunning()){
        Death::GetInstance()->Events();
        Death::GetInstance()->Update();
        Death::GetInstance()->Render();
        Timer::GetInstance()->Tick();
    }
}
void WinScreen(){
    Win::GetInstance()->Run();
    Win::GetInstance()->SetGameEndTrue();
    Engine::EndTime = clock();
    Win::GetInstance()->SetMinutes(((Engine::EndTime - Engine::StartTime) / CLOCKS_PER_SEC ) / 60);
    Win::GetInstance()->SetSecs(((Engine::EndTime - Engine::StartTime) / CLOCKS_PER_SEC ) % 60);
    Win::GetInstance()->SetTime();
    while(Win::GetInstance()->IsRunning()){
        Win::GetInstance()->Events();
        Win::GetInstance()->Update();
        Win::GetInstance()->Render();
        Timer::GetInstance()->Tick();
    }
}
void(*pause)() = StartPause;
void(*death)() = DeathScreen;
void(*win)() = WinScreen;

bool Engine::Init() {

    if(SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!= 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_ALLOW_HIGHDPI);
    m_Window = SDL_CreateWindow("Платформер", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    if(m_Window == nullptr){
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_Renderer == nullptr){
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }

    if(!MapParser::GetInstance()->Load("level1", "assets/maps/map1.tmx")){
        std::cout << "Failed to load map 1" << std::endl;
    }
    if(!MapParser::GetInstance()->Load("level2", "assets/maps/map2.tmx")){
        std::cout << "Failed to load map 2" << std::endl;
    }
    if(!MapParser::GetInstance()->Load("level3", "assets/maps/map3.tmx")){
        std::cout << "Failed to load map 3" << std::endl;
    }
    m_LevelMap1 = MapParser::GetInstance()->GetMap("level1");
    m_LevelMap2 = MapParser::GetInstance()->GetMap("level2");
    m_LevelMap3 = MapParser::GetInstance()->GetMap("level3");

    TextureManager::GetInstance()->Load("player", "assets/all_sprites.png");
    TextureManager::GetInstance()->Load("bg", "assets/BG.png");
    TextureManager::GetInstance()->Load("krip", "assets/Enemies/Idle4.png");
    TextureManager::GetInstance()->Load("krip_run", "assets/Enemies/Walk4.png");
    TextureManager::GetInstance()->Load("krip_dead", "assets/Enemies/Death.png");
    TextureManager::GetInstance()->Load("krip_attack", "assets/Enemies/Attack2.png");
    TextureManager::GetInstance()->Load("player_def1", "assets/Defend1.png");
    TextureManager::GetInstance()->Load("player_def2", "assets/Defend2.png");
    TextureManager::GetInstance()->Load("player_def3", "assets/Defend3.png");
    TextureManager::GetInstance()->Load("heart", "assets/heart.png");

    Sound::GetInstance()->LoadMusic("main", "assets/music.mp3");
    Sound::GetInstance()->LoadEffect("dmg", "assets/take_dmg.mp3");

    Record::GetInstance()->ReadFile();
    return m_IsRunning = true;
}

void Engine::Update() {
    Events();
    int temp;
    float dt = Timer::GetInstance()->GetDeltaTime();
    for (unsigned int i = 0; i < m_GameObjects.size(); i++) {
        temp = m_GameObjects[i]->Update(dt);
        if (temp == 1) {
            m_GameObjects[i]->Clean();
            m_GameObjects.erase(m_GameObjects.begin() + i);
        }
        if (temp == 2){
            m_CurLevel++;
            UnsetLevel();
            if(m_CurLevel == 2)
                SetLevel2();
            if(m_CurLevel == 3)
                SetLevel3();
            if(m_CurLevel == 4)
                win();
        }
        if (temp == 3)
            death();
    }
    if(m_CurLevel == 1)
       m_LevelMap1->Update();
    if(m_CurLevel == 2)
       m_LevelMap2->Update();
    if(m_CurLevel == 3)
        m_LevelMap3->Update();
        Camera::GetInstance()->Update(dt);
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_ESCAPE))
        pause();
}

void Engine::Render() {
    SDL_SetRenderDrawColor(m_Renderer, 12, 34, 123, 255);
    SDL_RenderClear(m_Renderer);
    TextureManager::GetInstance()->Draw("bg", 0 ,0, 1920, 1080, 1.5 );
    if(m_CurLevel == 1)
        m_LevelMap1->Render();
    if(m_CurLevel == 2)
        m_LevelMap2->Render();
    if(m_CurLevel == 3)
        m_LevelMap3->Render();
    for(unsigned int i = 0; i != m_GameObjects.size(); i++)
        m_GameObjects[i]->Draw();
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
    Input::GetInstance()->Listen();
//    if(!m_IsPaused && Input::GetInstance()->GetKeyDown(SDL_SCANCODE_ESCAPE)) {
//        m_IsPaused = true;
//        //SDL_Delay(200);
//    }
//    if(m_IsPaused && Input::GetInstance()->GetKeyDown(SDL_SCANCODE_F5)) {
//        m_IsPaused = false;
//        //SDL_Delay(200);
//    }
}

bool Engine::Clean() {
    for(unsigned int i = 0; i != m_GameObjects.size(); i++)
        m_GameObjects[i]->Clean();
    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
}

void Engine::Quit() {
    m_IsRunning = false;
}

GameMap* Engine::GetMap() {
    if(m_CurLevel == 1)
        return m_LevelMap1;
    if(m_CurLevel == 2)
        return m_LevelMap2;
    if(m_CurLevel == 3)
        return m_LevelMap3;
}

void Engine::UnsetLevel() {
    for (unsigned int i = 0; i < m_GameObjects.size(); i++){
        m_GameObjects[i]->Clean();
    }
    m_GameObjects.clear();
}

void Engine::SetLevel1() {
    StartTime = clock();
    Engine::GetInstance()->GetMap()->SetExitX(2358);
    Engine::GetInstance()->GetMap()->SetExitY(64);
    Warrior* player = new Warrior(new Proporties("player", 100, 100, 288, 128));
    Enemy* krip = new Enemy(new Proporties("krip", 300, 200, 48, 48));
    Enemy* krip2 = new Enemy(new Proporties("krip", 400, 200, 48, 48));
    Enemy* krip3 = new Enemy(new Proporties("krip", 450, 200, 48, 48));
    Enemy* krip4 = new Enemy(new Proporties("krip", 430, 513, 48, 48));
    Enemy* krip5 = new Enemy(new Proporties("krip", 500, 513, 48, 48));
    Enemy* krip6 = new Enemy(new Proporties("krip", 1600, 817, 48, 48));
    Enemy* krip7 = new Enemy(new Proporties("krip", 1800, 817, 48, 48));
    Enemy* krip8 = new Enemy(new Proporties("krip", 2000, 817, 48, 48));
    Enemy* krip9 = new Enemy(new Proporties("krip", 2200, 817, 48, 48));
    Enemy* krip10 = new Enemy(new Proporties("krip", 1950, 81, 48, 48));
    Enemy* krip11 = new Enemy(new Proporties("krip", 2000, 81, 48, 48));
    m_GameObjects.push_back(player);
    m_GameObjects.push_back(krip);
    m_GameObjects.push_back(krip2);
    m_GameObjects.push_back(krip3);
    m_GameObjects.push_back(krip4);
    m_GameObjects.push_back(krip5);
    m_GameObjects.push_back(krip6);
    m_GameObjects.push_back(krip7);
    m_GameObjects.push_back(krip8);
    m_GameObjects.push_back(krip9);
    m_GameObjects.push_back(krip10);
    m_GameObjects.push_back(krip11);
    Camera::GetInstance()->SetTarget(player->GetOrigin());

}

void Engine::SetLevel2() {
    Engine::GetInstance()->GetMap()->SetExitX(2370);
    Engine::GetInstance()->GetMap()->SetExitY(816);
    Warrior * player = new Warrior(new Proporties("player", 100, 100, 288, 128));
    Enemy* krip = new Enemy(new Proporties("krip", 730, 176, 48, 48));
    Enemy* krip2 = new Enemy(new Proporties("krip", 800, 176, 48, 48));
    Enemy* krip3 = new Enemy(new Proporties("krip", 900, 176, 48, 48));
    Enemy* krip4 = new Enemy(new Proporties("krip", 1330, 464, 48, 48));
    Enemy* krip5 = new Enemy(new Proporties("krip", 1390, 464, 48, 48));
    Enemy* krip6 = new Enemy(new Proporties("krip", 1450, 464, 48, 48));
    Enemy* krip7 = new Enemy(new Proporties("krip", 1560, 464, 48, 48));
    Enemy* krip8 = new Enemy(new Proporties("krip", 320, 640, 48, 48));
    Enemy* krip9 = new Enemy(new Proporties("krip", 400, 640, 48, 48));
    Enemy* krip10 = new Enemy(new Proporties("krip", 500, 640, 48, 48));
    Enemy* krip11 = new Enemy(new Proporties("krip", 600, 640, 48, 48));
    Enemy* krip12 = new Enemy(new Proporties("krip", 700, 640, 48, 48));
    Enemy* krip13 = new Enemy(new Proporties("krip", 800, 640, 48, 48));
    Enemy* krip14 = new Enemy(new Proporties("krip", 1600, 640, 48, 48));
    Enemy* krip15 = new Enemy(new Proporties("krip", 1700, 640, 48, 48));
    Enemy* krip16 = new Enemy(new Proporties("krip", 1800, 640, 48, 48));

    m_GameObjects.push_back(player);
    m_GameObjects.push_back(krip);
    m_GameObjects.push_back(krip2);
    m_GameObjects.push_back(krip3);
    m_GameObjects.push_back(krip4);
    m_GameObjects.push_back(krip5);
    m_GameObjects.push_back(krip6);
    m_GameObjects.push_back(krip7);
    m_GameObjects.push_back(krip8);
    m_GameObjects.push_back(krip9);
    m_GameObjects.push_back(krip10);
    m_GameObjects.push_back(krip11);
    m_GameObjects.push_back(krip12);
    m_GameObjects.push_back(krip13);
    m_GameObjects.push_back(krip14);
    m_GameObjects.push_back(krip15);
    m_GameObjects.push_back(krip16);
    Camera::GetInstance()->SetTarget(player->GetOrigin());
}

void Engine::SetLevel3() {
    Engine::GetInstance()->GetMap()->SetExitX(2365);
    Engine::GetInstance()->GetMap()->SetExitY(-32);
    Warrior * player = new Warrior(new Proporties("player", 100, 100, 288, 128));
    Enemy* krip = new Enemy(new Proporties("krip", 190, 816, 48, 48));
    Enemy* krip2 = new Enemy(new Proporties("krip", 250, 816, 48, 48));
    Enemy* krip3 = new Enemy(new Proporties("krip", 300, 816, 48, 48));
    Enemy* krip4 = new Enemy(new Proporties("krip", 400, 816, 48, 48));
    Enemy* krip5 = new Enemy(new Proporties("krip", 500, 816, 48, 48));
    Enemy* krip6 = new Enemy(new Proporties("krip", 1000, 816, 48, 48));
    Enemy* krip7 = new Enemy(new Proporties("krip", 1200, 816, 48, 48));
    Enemy* krip8 = new Enemy(new Proporties("krip", 1300, 816, 48, 48));
    Enemy* krip9 = new Enemy(new Proporties("krip", 1800, 817, 48, 48));
    Enemy* krip10 = new Enemy(new Proporties("krip", 1900, 817, 48, 48));
    Enemy* krip11 = new Enemy(new Proporties("krip", 2000, 817, 48, 48));
    Enemy* krip12 = new Enemy(new Proporties("krip", 2100, 817, 48, 48));
    Enemy* krip13 = new Enemy(new Proporties("krip", 2200, 817, 48, 48));
    Enemy* krip14 = new Enemy(new Proporties("krip", 2400, 817, 48, 48));
    Enemy* krip15 = new Enemy(new Proporties("krip", 2150, 496, 48, 48));
    Enemy* krip16 = new Enemy(new Proporties("krip", 2200, 496, 48, 48));
    Enemy* krip17 = new Enemy(new Proporties("krip", 2250, 496, 48, 48));
    Enemy* krip18 = new Enemy(new Proporties("krip", 1800, 224, 48, 48));
    Enemy* krip19 = new Enemy(new Proporties("krip", 1900, 224, 48, 48));
    Enemy* krip20 = new Enemy(new Proporties("krip", 1780, 0, 48, 48));
    Enemy* krip21 = new Enemy(new Proporties("krip", 1920, 0, 48, 48));
    m_GameObjects.push_back(player);
    m_GameObjects.push_back(krip);
    m_GameObjects.push_back(krip2);
    m_GameObjects.push_back(krip3);
    m_GameObjects.push_back(krip4);
    m_GameObjects.push_back(krip5);
    m_GameObjects.push_back(krip6);
    m_GameObjects.push_back(krip7);
    m_GameObjects.push_back(krip8);
    m_GameObjects.push_back(krip9);
    m_GameObjects.push_back(krip10);
    m_GameObjects.push_back(krip11);
    m_GameObjects.push_back(krip12);
    m_GameObjects.push_back(krip13);
    m_GameObjects.push_back(krip14);
    m_GameObjects.push_back(krip15);
    m_GameObjects.push_back(krip16);
    m_GameObjects.push_back(krip17);
    m_GameObjects.push_back(krip18);
    m_GameObjects.push_back(krip19);
    m_GameObjects.push_back(krip20);
    m_GameObjects.push_back(krip21);
    Camera::GetInstance()->SetTarget(player->GetOrigin());
}

