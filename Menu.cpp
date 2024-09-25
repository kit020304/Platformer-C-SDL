#include "Menu.h"
#include "TextureManager.h"
#include "Button.h"
#include "Timer.h"
#include "TextOutput.h"
#include "Sound.h"
#include <fstream>

Menu *Menu::s_Instance = nullptr;
std::string name = "Воин";
std::vector<std::string> Buttons;
Button* button_home_menu = nullptr;
Button* button_quit_menu = nullptr;
Button* player_name = nullptr;
Button* button_record = nullptr;

float t = -1;
void Read() {

    name=TextOutput::GetInstance()->Read(Menu::GetInstance()->GetWindowWidth() / 2 - 35, Menu::GetInstance()->GetWindowHeight() / 2+5-100,name);
    //name = Text::GetInstance()->Read();
    t = 40.0;
    SDL_Event events[100];
    SDL_PeepEvents(events,100,SDL_GETEVENT,-1,100);
    SDL_Event ev;
    while (SDL_PollEvent(&ev));
    Input::GetInstance()->SetLKM();
}

void FalseRanningMenu()
{
    Menu::GetInstance()->Quit();
    Engine::GetInstance()->Run();
    Win::GetInstance()->SetGameEndFalse();
    Win::GetInstance()->SetRecAdded();
    Engine::GetInstance()->SetCurLevel();
    Engine::GetInstance()->SetLevel1();
}

void AllQuit() {
    Engine::GetInstance()->Quit();
    Pause::GetInstance()->Quit();
    Menu::GetInstance()->Quit();
}
int flag_print = 0;
void StartRecord()
{
    flag_print = 0;
    Record::GetInstance()->Run();
    while (Record::GetInstance()->IsRunning())
    {
        Record::GetInstance()->Events();
        Record::GetInstance()->Update();
        Record::GetInstance()->Render();
        Timer::GetInstance()->Tick();
    }
}

bool Menu::Init(SDL_Renderer *renderer, SDL_Window *window) {
    void (*callback)() = FalseRanningMenu;
    void (*quit)() = AllQuit;
    void (*read)() = Read;
    void (*record)() = StartRecord;
    m_WindowHeight = SCREEN_HEIGHT;
    m_WindowWidth = SCREEN_WIDTH;
    m_Renderer = renderer;
    m_Window = window;
    TextureManager::GetInstance()->Load("menu_bg", "assets/menu_back.png");
    TextureManager::GetInstance()->Load("main_n", "assets/n.png");
    Buttons.push_back("main_n");
    TextureManager::GetInstance()->Load("main_h", "assets/h.png");
    Buttons.push_back("main_h");
    TextureManager::GetInstance()->Load("main_c", "assets/c.png");
    Buttons.push_back("main_c");

    Sound::GetInstance()->PlayMusic("main");
    button_home_menu = new Button(m_WindowWidth / 2 - 50, m_WindowHeight / 2 - 150, 96, 32, callback, Buttons,0.7);
    button_quit_menu = new Button(m_WindowWidth / 2 - 50, m_WindowHeight / 2+0, 96, 32, quit, Buttons, 0.7);
    player_name=new Button(m_WindowWidth / 2 - 50, m_WindowHeight / 2-75, 96, 32, read, Buttons, 0.7);
    button_record=new Button(m_WindowWidth / 2 - 50, m_WindowHeight / 2 + 75, 96, 32, record, Buttons, 0.7);

    return m_IsRunning = true;
}

void Menu::Quit() {
    m_IsRunning = false;
}

void Menu::Update() {
    float dt = Timer::GetInstance()->GetDeltaTime();
    SDL_GL_GetDrawableSize(m_Window, &m_WindowWidth, &m_WindowHeight);
    float dtt = Timer::GetInstance()->GetDeltaTime();
    t -= dtt;
    if (t < 0)
        button_home_menu->Update(dt,m_WindowWidth/2-50,m_WindowHeight/2-200);
    button_record->Update(dt, m_WindowWidth / 2 - 50, m_WindowHeight / 2 + 0);
    button_quit_menu->Update(dt, m_WindowWidth / 2 -50, m_WindowHeight / 2+100);
    player_name->Update(dt, m_WindowWidth / 2 -50, m_WindowHeight / 2-100);
}

void Menu::Render() {
    TextureManager::GetInstance()->Draw("menu_bg", 0, 0, 1920, 960, 1, 1, 0);
    button_home_menu->Draw();
    button_quit_menu->Draw();
    button_record->Draw();
    player_name->Draw();
    TextOutput::GetInstance()->Write(name, m_WindowWidth / 2 - 35, m_WindowHeight / 2 -100+ 5,15, 0, 0, 0);
    TextOutput::GetInstance()->Write("Играть", m_WindowWidth / 2 - 45, m_WindowHeight / 2 - 200, 25, 0, 0, 0);
    TextOutput::GetInstance()->Write("Выход", m_WindowWidth / 2 - 45, m_WindowHeight / 2 + 100, 25, 0, 0, 0);
    TextOutput::GetInstance()->Write("Рекорды", m_WindowWidth / 2 - 50, m_WindowHeight / 2, 23, 0, 0, 0);
    TextOutput::GetInstance()->Write("Главное Меню", m_WindowWidth / 2 - 113, 0, 50, 255, 0 ,0);
    TextOutput::GetInstance()->Write("Курсовая работа на тему 'Платформер'", 1170, 850, 35, 255, 0, 0);
    TextOutput::GetInstance()->Write("Пучель В.А. 0725Б", 1400, 900, 33, 255, 0, 0);
    SDL_RenderPresent(m_Renderer);
}

void Menu::Events() {
    Input::GetInstance()->Listen();
}



Pause* Pause::s_Instance = nullptr;
Button* button_home_pause = nullptr;
Button* button_home_pausetomenu = nullptr;
void FalseRanningPause()
{
    Pause::GetInstance()->Quit();
}
void FalseRanningPausetoMenu() {
    //Records::GetInstance()->add(name, hunter->GetPoints());
    Pause::GetInstance()->Quit();
    Engine::GetInstance()->Stop();
    Menu::GetInstance()->Run();
}

bool Pause::Init(SDL_Renderer* renderer, SDL_Window* window)
{
    void (*callback)() = FalseRanningPause;
    void (*callbacktomenu)() = FalseRanningPausetoMenu;
    m_WindowHeight = SCREEN_HEIGHT;
    m_WindowWidth = SCREEN_WIDTH;
    m_Renderer = renderer;
    m_Window = window;
    TextureManager::GetInstance()->Load("mouse", "assets/mouse.png");
    TextureManager::GetInstance()->Load("keyboard", "assets/keyboard2.png");

    button_home_pause = new Button(m_WindowWidth / 2 - 50, m_WindowHeight / 2 - 100, 96, 32, callbacktomenu, Buttons);
    button_home_pausetomenu = new Button(m_WindowWidth / 2 - 50, m_WindowHeight / 2, 96, 32, callback, Buttons);
    return m_IsRunning = true;
}



void Pause::Quit()
{
    EndPause = clock();
    Engine::StartTime -= ( StartPause - EndPause);
    m_IsRunning = false;
}

void Pause::Update()
{
    float dt = Timer::GetInstance()->GetDeltaTime();
    button_home_pause->Update(dt,m_WindowWidth/2-50,m_WindowHeight/2);
    button_home_pausetomenu->Update(dt, m_WindowWidth / 2 - 50, m_WindowHeight / 2-100);
}

void Pause::Render()
{

    TextureManager::GetInstance()->Draw("mouse", 1300, m_WindowHeight/2 - 400, 999, 2141, 0.4, 0.3, 0);
    TextureManager::GetInstance()->Draw("keyboard", 0, m_WindowHeight/2 - 400, 1024, 1024, 0.9, 0.9, 0);
    button_home_pause->Draw();
    button_home_pausetomenu->Draw();
    TextOutput::GetInstance()->Write("Ускорение", 40, m_WindowHeight / 2 + 125, 35, 0, 0, 0);
    TextOutput::GetInstance()->Write("Прыжок", 360, m_WindowHeight / 2 + 185, 35, 0, 0, 0);
    TextOutput::GetInstance()->Write("Перемещение", 160, m_WindowHeight / 2+60, 35, 0, 0, 0);
    TextOutput::GetInstance()->Write("Атака", 1400, m_WindowHeight / 2 - 150, 35, 0, 0, 0);
    TextOutput::GetInstance()->Write("Защита", 1520, m_WindowHeight / 2 - 150, 35, 0, 0, 0);
    TextOutput::GetInstance()->Write("Продолжить игру", m_WindowWidth / 2 - 45, m_WindowHeight / 2 - 95, 15, 0, 0, 0);
    TextOutput::GetInstance()->Write("Вернуться в меню", m_WindowWidth / 2 - 45, m_WindowHeight / 2 + 5 , 15, 0, 0, 0);
    TextOutput::GetInstance()->Write("Пауза", m_WindowWidth / 2 - 50, 0, 50, 255, 0 ,0);
    SDL_RenderPresent(m_Renderer);
}

void Pause::Events()
{
    Input::GetInstance()->Listen();
}


Death* Death::s_Instance = nullptr;
Button* button_home_death = nullptr;
inline void DeathToMenu() {
    //Records::GetInstance()->add(name, hunter->GetPoints());
    Death::GetInstance()->Quit();
    Engine::GetInstance()->Stop();
    Menu::GetInstance()->Run();
}
bool Death::Init(SDL_Renderer* renderer, SDL_Window* window)
{
    void (*death_to_menu)() = DeathToMenu;
    m_WindowHeight = SCREEN_HEIGHT;
    m_WindowWidth = SCREEN_WIDTH;
    m_Renderer = renderer;
    m_Window = window;


    button_home_death = new Button(m_WindowWidth / 2 - 50, m_WindowHeight / 2 - 100, 96, 32, death_to_menu, Buttons);
    return m_IsRunning = true;
}



void Death::Quit()
{
    m_IsRunning = false;
}

void Death::Update()
{
    float dt = Timer::GetInstance()->GetDeltaTime();
    button_home_death->Update(dt, m_WindowWidth / 2 - 50, m_WindowHeight / 2 - 100);
}

void Death::Render()
{
    button_home_death->Draw();
    TextOutput::GetInstance()->Write("Вернуться в меню", m_WindowWidth / 2 - 45, m_WindowHeight / 2 - 95, 15, 0, 0, 0);
    TextOutput::GetInstance()->Write("Потрачено", m_WindowWidth / 2 - 140, m_WindowHeight/2 - 200, 100, 255, 0 ,0);
    SDL_RenderPresent(m_Renderer);
}

void Death::Events()
{
    Input::GetInstance()->Listen();
}



Win* Win::s_Instance = nullptr;
Button* button_home_Win = nullptr;

void WinToMenu() {
    //Records::GetInstance()->add(name, hunter->GetPoints());
    Engine::GetInstance()->UnsetLevel();
    Engine::GetInstance()->Stop();
    Win::GetInstance()->Quit();
    Menu::GetInstance()->Run();
}


bool Win::Init(SDL_Renderer* renderer, SDL_Window* window)
{
    void (*callbacktomenu)() = WinToMenu;

    m_WindowHeight = SCREEN_HEIGHT;
    m_WindowWidth = SCREEN_WIDTH;
    m_Renderer = renderer;
    m_Window = window;


    button_home_Win = new Button(m_WindowWidth / 2 - 50, m_WindowHeight / 2 -100, 96, 32, callbacktomenu, Buttons);
    return m_IsRunning = true;
}



void Win::Quit()
{
    m_IsRunning = false;
}

void Win::Update()
{
    float dt = Timer::GetInstance()->GetDeltaTime();
    button_home_Win->Update(dt, m_WindowWidth / 2 - 50, m_WindowHeight / 2-100);
}

void Win::Render()
{
    button_home_Win->Draw();
    TextOutput::GetInstance()->Write("Вернуться в меню", m_WindowWidth / 2 - 45, m_WindowHeight / 2 - 95, 15, 0, 0, 0);
    TextOutput::GetInstance()->Write("Поздравляем, вы прошли игру!", m_WindowWidth / 2 - 50, 0, 50, 255, 0 ,0);
    TextOutput::GetInstance()->Write("Затраченное время:", m_WindowWidth / 2 - 50, 100, 45, 255, 0 ,0);
    TextOutput::GetInstance()->Write(Time, m_WindowWidth / 2 - 50, 200, 45, 255, 0 ,0);
    SDL_RenderPresent(m_Renderer);
}

void Win::Events()
{
    Input::GetInstance()->Listen();
    if(m_GameEnded && !m_IsRecAdded){
        Record::GetInstance()->add(name, Minutes, Seconds);
        m_IsRecAdded = true;
    }
}

Record* Record::s_Instance = nullptr;
Button* button_quit_record = nullptr;
Button* button_clear_record = nullptr;
Button* button_print_record = nullptr;
float time_print = 45.0;
void Print(){
    time_print = 45.0;
    flag_print = 1;
    Record::GetInstance()->CreateFile();
}
void ClearRec(){
    Record::GetInstance()->Clear();
}
void FalseRanningRecord()
{
    Record::GetInstance()->Quit();
}
bool Record::Init(SDL_Renderer *renderer, SDL_Window *window) {
    m_WindowHeight = SCREEN_HEIGHT;
    m_WindowWidth = SCREEN_WIDTH;
    m_Renderer = renderer;
    m_Window = window;
    void(*clear)() = ClearRec;
    void (*print)() = Print;
    void(*quit)() = FalseRanningRecord;
    button_quit_record= new Button(m_WindowWidth / 2 - 50, m_WindowHeight - 100, 96, 32, quit, Buttons);
    button_clear_record = new Button(m_WindowWidth / 2 - 200, m_WindowHeight - 100, 96, 32, clear, Buttons);
    button_print_record = new Button(m_WindowWidth / 2 + 200-96, m_WindowHeight - 100, 96, 32, print, Buttons);
    return m_IsRunning = true;
}
void Record::Quit() {
    m_IsRunning = false;
}
void Record::Update() {
    float dt = Timer::GetInstance()->GetDeltaTime();;
    button_quit_record->Update(dt, m_WindowWidth / 2 - 50, m_WindowHeight - 100);
    button_clear_record->Update(dt, m_WindowWidth / 2 - 200, m_WindowHeight - 100);
    button_print_record->Update(dt, m_WindowWidth / 2 + 200-96, m_WindowHeight - 100);
}
void Record::Render() {
    TextureManager::GetInstance()->Draw("menu_bg", 0, 0, 1920, 960, 1, 1, 0);
    TextOutput::GetInstance()->Write("Рекорды", m_WindowWidth / 2 - 50, 0, 50, 255, 0 ,0);
    if(flag_print){
        if(time_print != 0){
            TextOutput::GetInstance()->Write("файл сохранен в папку проекта", m_WindowWidth /10*4 + 70, m_WindowHeight-30, 16,255,255,255);
            float dt = Timer::GetInstance()->GetDeltaTime();
            time_print > 0 ? time_print -= dt : time_print = 0;
        }
        else
            flag_print = 0;
    }
    button_quit_record->Draw();
    button_print_record->Draw();
    button_clear_record->Draw();
    for(int i = 0; i < m_records.size(); i++){
        TextOutput::GetInstance()->Write(m_records[i].name1 + ' ' + std::to_string(m_records[i].mins) + ':' + std::to_string(m_records[i].secs), 205, (i+1)*100, 30, 0, 0, 0);
    }
    TextOutput::GetInstance()->Write("Вернуться в меню", m_WindowWidth / 2 - 45, m_WindowHeight - 95 , 15, 0, 0, 0);
    TextOutput::GetInstance()->Write("Очистить список", m_WindowWidth / 2 - 195, m_WindowHeight - 95 , 15, 0, 0, 0);
    TextOutput::GetInstance()->Write("Печать списка", m_WindowWidth / 2 + 110, m_WindowHeight - 95 , 15, 0, 0, 0);
    SDL_RenderPresent(m_Renderer);
}
void Record::Events() {
    Input::GetInstance()->Listen();
}
void Record::CreateFile() {
    std::ofstream out("Platformer.txt", std::ios::out);
    if(m_records.size() != 0){
        for(int i = 0; i < m_records.size(); i++){
            out.write(m_records[i].name1.c_str(), m_records[i].name1.length());
            out << ' ';
            out << m_records[i].mins;
            out << ' ';
            out << m_records[i].secs;
            out << '\n';
        }
        out.close();
    }
}

void Record::add(std::string name2, long mins1, long secs1) {
    data temp;
    temp.name1 = name2;
    temp.mins = mins1;
    temp.secs = secs1;
    m_records.push_back(temp);
}

void Record::ReadFile() {
    std::ifstream in("Platformer.txt", std::ios::in);
    data temp;
    while (in >> temp.name1 >> temp.mins >> temp.secs) {
        m_records.push_back(temp);
    }
}