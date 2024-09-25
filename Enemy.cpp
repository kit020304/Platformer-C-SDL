#include "Enemy.h"
#include "Camera.h"
#include "CollisionHandler.h"
#include "ObjectFactory.h"
#include "TextureManager.h"
#include "Warrior.h"


static Registrar<Enemy> registrar("KRIP");

Enemy::Enemy(Proporties *props) : Character(props) {
    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(9.8f);
    m_Collider = new Collider();
    m_AttackCollision = new Collider();
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 0, 4, 150);
    m_WaitTime = 10 + rand() % 300;
    m_TempTime = m_WaitTime;
}

void Enemy::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_Flip);
    //m_Collider->Draw();
    //m_AttackCollision->Draw();
}

int Enemy::Update(float dt) {
    m_RigidBody->UnSetForce();
    m_AttackCollision->UnSet();
    if(!IsHeroNear() && !m_IsDead) {
        if (m_TempTime > 0)
            m_TempTime -= dt;
        if (m_TempTime <= 0 && m_RunTime > 0 && !m_IsDead) {
            m_IsRunning = true;
            m_RunTime -= dt;
            if (!m_Side) {
                m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE_krip);
                m_Flip = SDL_FLIP_NONE;
            }
            else {
                m_Flip = SDL_FLIP_HORIZONTAL;
                m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE_krip);
            }
        }
        if (m_RunTime <= 0) {
            m_RunTime = RUN_TIME;
            m_TempTime = m_WaitTime;
            m_Side = rand() % 2;
            m_IsRunning = false;
        }
    }
    if(CollisionHandler::GetInstance()->CheckCollision(m_Collider->Get(), Warrior::s_AttackCollision->Get())){
        m_RigidBody->UnSetForce();
        m_IsDead = true;
        m_IsRunning = false;
    }
    if(m_IsAttacking && m_AttackTime > 0)
        m_AttackTime -= dt;
    else{
        m_IsAttacking = false;
        m_AttackTime = ATTACK_TIME_krip;
    }
    if(m_IsAttacking && m_Flip == SDL_FLIP_NONE && m_AttackTime < ATTACK_TIME_krip / 1.2) {
        m_AttackCollision->Set(m_Transform->X + 16 + 13, m_Transform->Y, 30, 48);
        if (CollisionHandler::GetInstance()->CheckCollision(m_AttackCollision->Get(), Warrior::s_Collider->Get()) && !(Warrior::s_Defendingfl && m_Flip != Warrior::s_DefSide) && m_AttackTime <= 0)
            Warrior::s_IsTakenHit = true;
    }
    else
    if(m_IsAttacking && m_Flip ==  SDL_FLIP_HORIZONTAL && m_AttackTime < ATTACK_TIME_krip / 1.2){
        m_AttackCollision->Set(m_Transform->X+11-18, m_Transform->Y, 30, 48);
        if(CollisionHandler::GetInstance()->CheckCollision(m_AttackCollision->Get(), Warrior::s_Collider->Get()) && !(Warrior::s_Defendingfl && m_Flip != Warrior::s_DefSide) && m_AttackTime <= 0)
            Warrior::s_IsTakenHit = true;
    }
    else
        m_AttackCollision->UnSet();
    if(m_IsDead)
        m_DeathTime -=dt;
    if(m_DeathTime <= 0)
        return 1;
    m_RigidBody->Update(dt);
    m_LastSafePosition.X = m_Transform->X;
    m_Transform->X += m_RigidBody->Position().X;
    m_Collider->Set(m_Transform->X+11, m_Transform->Y-4, 24, 48);
    if(CollisionHandler::GetInstance()->MapCollision(m_Collider->Get(), 1))
        m_Transform->X = m_LastSafePosition.X;
    m_RigidBody->Update(dt);
    m_LastSafePosition.Y = m_Transform->Y;
    m_Transform->Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform->X+11, m_Transform->Y-4, 24, 48);
    if(CollisionHandler::GetInstance()->MapCollision(m_Collider->Get(), 1))
        m_Transform->Y = m_LastSafePosition.Y;
    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    AnimationState();
    m_Animation->Update();
    return 0;
}

void Enemy::AnimationState() {
    m_Animation->SetProps("krip", 0, 4, 150);
    if (m_IsRunning) {
        if (m_IsSprinting)
            m_Animation->SetProps("krip_run", 0, 6, 75);
        else
            m_Animation->SetProps("krip_run", 0, 6, 100);
    }
    if(m_IsAttacking)
        m_Animation->SetProps("krip_attack", 0, 6, 165);
    if(m_IsDead) {
        m_Animation->SetProps("krip_dead", 0, 6, 450);
    }
}
void Enemy::Clean() {
    //TextureManager::GetInstance()->Drop("krip_dead");
    delete m_RigidBody;
    delete m_Collider;
    delete m_Animation;
}

bool Enemy::IsHeroNear() {
    if (!m_IsDead) {
        if (m_Collider->Get().x - Warrior::s_Collider->Get().x < 300 &&
            m_Collider->Get().x - Warrior::s_Collider->Get().x > 0 &&
            abs(m_Collider->Get().y - Warrior::s_Collider->Get().y) < 150) {
            m_Flip = SDL_FLIP_HORIZONTAL;
            if (m_Collider->Get().x - Warrior::s_Collider->Get().x < 30 &&
                abs(m_Collider->Get().y - Warrior::s_Collider->Get().y) < 30) {
                m_RigidBody->UnSetForce();
                m_IsAttacking = true;
                return true;
            }
            m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE_krip * 1.5);
            m_IsRunning = true;
            m_IsSprinting = true;
            return true;
        }
        if (m_Collider->Get().x - Warrior::s_Collider->Get().x > -300 &&
            m_Collider->Get().x - Warrior::s_Collider->Get().x < 0 &&
            abs(m_Collider->Get().y - Warrior::s_Collider->Get().y) < 150) {
            m_Flip = SDL_FLIP_NONE;
            if (m_Collider->Get().x - Warrior::s_Collider->Get().x > -30 &&
                abs(m_Collider->Get().y - Warrior::s_Collider->Get().y) < 30) {
                m_RigidBody->UnSetForce();
                m_IsAttacking = true;
                return true;
            }
            m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE_krip * 1.5);
            m_IsRunning = true;
            m_IsSprinting = true;
            return true;
        }
    }
    m_IsSprinting = false;
    m_IsRunning = false;
    return false;
}