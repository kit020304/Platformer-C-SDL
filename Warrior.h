#ifndef KURSACH2_WARRIOR_H
#define KURSACH2_WARRIOR_H
#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Enemy.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 20.0f
#define RUN_FORCE 3.0f
#define ATTACK_TIME 45.0f
#define PREPARE_TO_DEF 7.0f
#define AFTER_DEF 7.0f
#define TAKE_HIT_TIME 10.0f
#define HP_COUNT 3
#define DEATH_TIME 49.0f

class Warrior: public Character{
public:
    Warrior(Proporties* props);

    virtual void Draw();
    virtual void Clean();
    virtual int Update(float dt);
private:
    friend class Enemy;
    void AnimationState();

    int m_HP = HP_COUNT;

    bool m_IsRunning;
    bool m_IsJumping;
    bool m_IsFalling;
    bool m_IsGrounded;
    bool m_IsAttacking = false;
    bool m_IsSprinting = false;
    bool m_IsAirAttacking = false;
    bool m_IsDefending = false;
    bool m_PrepDef = false;
    bool m_AfterDef = false;
    bool m_IsDead = false;

    float m_JumpTime;
    float m_JumpForce;
    float m_AttackTime = ATTACK_TIME;
    float m_PrepareDefTime = PREPARE_TO_DEF;
    float m_AfterDefTime = AFTER_DEF;
    float m_TakeHitTime = TAKE_HIT_TIME;
    float m_DeathTime = DEATH_TIME;

    static int frame;
    static Collider* s_Collider;
    static Collider* s_AttackCollision;
    static bool s_Defendingfl;
    static bool s_IsTakenHit;

    Animation* m_Animation;
    RigidBody* m_RigidBody;
    Vector2D m_LastSafePosition;
    static SDL_RendererFlip s_DefSide;
};


#endif //KURSACH2_WARRIOR_H
