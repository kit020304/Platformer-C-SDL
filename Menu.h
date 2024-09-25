#ifndef KURSACH2_MENU_H
#define KURSACH2_MENU_H
#include <SDL.h>
#include "Engine.h"
#include "ctime"
#include <string>
class Menu {
public:
    static Menu* GetInstance(){return s_Instance = (s_Instance != nullptr) ? s_Instance : new Menu();}
    bool Init(SDL_Renderer* renderer, SDL_Window* window);
    void Quit();
    void Update();
    void Render();
    void Events();
    inline bool IsRunning(){return m_IsRunning; }
    inline int GetWindowWidth(){return m_WindowWidth;}
    inline int GetWindowHeight(){return m_WindowHeight;}
    inline void Run(){m_IsRunning = true;}
private:
    SDL_Window* m_Window;
    int m_WindowHeight;
    int m_WindowWidth;
    SDL_Renderer* m_Renderer;
    bool m_IsRunning;
    Menu(){}
    static Menu* s_Instance;
};

class Pause {
public:
    static Pause* GetInstance() {
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Pause();
    }

    bool Init(SDL_Renderer* m_Renderer, SDL_Window* m_Window);
    inline void Run() { m_IsRunning = true; StartPause = clock();}

    void Quit();

    void Update();

    void Render();

    void Events();

    inline bool IsRunning() { return m_IsRunning; };
private:
    long int StartPause;
    long int EndPause;
    SDL_Window* m_Window;
    int m_WindowHeight;
    int m_WindowWidth;
    SDL_Renderer* m_Renderer;
    bool m_IsRunning;
    Pause() {};
    static Pause* s_Instance;
};

class Death {
public:
    static Death* GetInstance(){return s_Instance = (s_Instance != nullptr) ? s_Instance : new Death();}
    bool Init(SDL_Renderer* renderer, SDL_Window* window);
    inline void Run(){m_IsRunning = true;}
    void Quit();
    void Update();
    void Render();
    void Events();
    inline bool IsRunning(){return m_IsRunning;}

private:
    SDL_Window* m_Window;
    int m_WindowHeight;
    int m_WindowWidth;
    SDL_Renderer* m_Renderer;
    bool m_IsRunning;
    Death(){}
    static Death* s_Instance;
};

class Win {
public:
    static Win* GetInstance(){return s_Instance = (s_Instance != nullptr) ? s_Instance : new Win();}
    bool Init(SDL_Renderer* renderer, SDL_Window* window);
    void Quit();
    void Update();
    void Render();
    void Events();
    void SetMinutes(long int mins){Minutes = mins;}
    void SetSecs(long int secs){Seconds = secs;}
    void SetTime(){Time = std::to_string(Minutes) + ":" + std::to_string(Seconds);}
    void SetGameEndTrue(){m_GameEnded = true;}
    void SetGameEndFalse(){m_GameEnded = false;}
    void SetRecAdded(){m_IsRecAdded = false;}
    inline bool IsRunning(){return m_IsRunning;}
    inline void Run(){m_IsRunning = true;}
private:
    SDL_Window* m_Window;
    long int Minutes;
    long int Seconds;
    std::string Time;
    int m_WindowHeight;
    int m_WindowWidth;
    bool m_IsRecAdded = false;
    SDL_Renderer* m_Renderer;
    bool m_IsRunning;
    bool m_GameEnded = false;
    Win(){}
    static Win* s_Instance;
};

struct data{
    std::string name1;
    long int mins;
    long int secs;
};
class Record{
public:
    static Record* GetInstance(){return s_Instance = (s_Instance != nullptr) ? s_Instance : new Record();}
    bool Init(SDL_Renderer* renderer, SDL_Window* window);
    inline void Run(){m_IsRunning = true;}
    void CreateFile();
    void add(std::string name1, long int mins, long int secs);
    void ReadFile();
    void Clear(){m_records.clear();}
    void Quit();
    void Update();
    void Render();
    void Events();
    inline bool IsRunning(){return m_IsRunning;}
private:
    SDL_Window* m_Window;
    std::vector<data> m_records;
    int m_WindowHeight;
    int m_WindowWidth;
    SDL_Renderer* m_Renderer;
    bool m_IsRunning;
    Record(){}
    static Record* s_Instance;
};




#endif //KURSACH2_MENU_H
