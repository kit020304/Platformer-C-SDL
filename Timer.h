#ifndef KURSACH2_TIMER_H
#define KURSACH2_TIMER_H
const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 0.75f;
class Timer {
public:
     inline static Timer* GetInstance(){
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Timer();
    };
    void Tick();
    inline float GetDeltaTime(){return m_DeltaTime;}

private:
    Timer(){};
     static Timer * s_Instance;
     float m_DeltaTime = 0.0f;
     float m_LastTime = 0.0f;
};


#endif //KURSACH2_TIMER_H
