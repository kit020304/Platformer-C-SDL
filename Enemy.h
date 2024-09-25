#ifndef KURSACH2_ENEMY_H
#define KURSACH2_ENEMY_H
#include "Character.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Animation.h"
#include <vector>

#define RUN_FORCE_krip 2.0f
#define RUN_TIME 150.0f
#define DEATH_TIME_krip 47.0f
#define ATTACK_TIME_krip 60.0f

class Enemy: public Character {
public:
    Enemy(Proporties* props);
    virtual void Draw();
    virtual void Clean();
    virtual int Update(float dt);

private:
    void AnimationState();
    bool IsHeroNear();

    Collider* m_Collider;
    Collider* m_AttackCollision;
    RigidBody* m_RigidBody;
    Animation* m_Animation;
    Vector2D m_LastSafePosition;
    float m_RunTime;
    float m_WaitTime;
    float m_DeathTime = DEATH_TIME_krip;
    float m_AttackTime = ATTACK_TIME_krip;
    float m_TempTime;

    bool m_IsAttacking = false;
    bool m_IsRunning;
    bool m_IsSprinting = false;
    bool m_IsDead = false;
    bool m_Side;
};

#endif //KURSACH2_ENEMY_H
