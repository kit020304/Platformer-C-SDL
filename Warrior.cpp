#include "Warrior.h"
#include "TextureManager.h"
#include "Input.h"
#include "Engine.h"
#include "CollisionHandler.h"
#include "ObjectFactory.h"
#include <iostream>
#include "Sound.h"

int Warrior::frame = 0;
static Registrar<Warrior> registrar("PLAYER");
Collider* Warrior::s_AttackCollision = nullptr;
Collider* Warrior::s_Collider = nullptr;
bool Warrior::s_Defendingfl = false;
bool Warrior::s_IsTakenHit = false;
SDL_RendererFlip Warrior::s_DefSide = SDL_FLIP_NONE;

Warrior::Warrior(Proporties* props) : Character(props){
    m_JumpForce = JUMP_FORCE;
    m_JumpTime = JUMP_TIME;
    s_Collider = new Collider();
    s_Collider->SetBuffer(-130, -64, 0, 0);
    s_AttackCollision = new Collider();
    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(7.0f);
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 0, 8, 100);
}

void Warrior::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_Flip);
    //s_Collider->Draw();
    //s_AttackCollision->Draw();
    for(int i = 1; i <= 30 * m_HP; i+= 30)
        TextureManager::GetInstance()->Draw("heart", 0 + i, 0, 512, 512, 0.05, 0.05, 0);

}

int Warrior::Update(float dt) {
    m_IsRunning = false;
    m_IsDefending = false;
    m_RigidBody->UnSetForce();
    if (frame != 0){
        frame--;
    }
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD && !m_IsAttacking && !frame && !m_IsDefending && !s_Defendingfl && !m_IsDead){
        if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LSHIFT)) {
            m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE * 1.7);
            m_IsSprinting = true;
        }
        else {
            m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
            m_IsSprinting = false;
        }
        m_Flip = SDL_FLIP_NONE;
        m_IsRunning = true;
    }
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD && !m_IsAttacking && !frame && !m_IsDefending && !s_Defendingfl && !m_IsDead){
        if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LSHIFT)) {
            m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE * 1.7);
            m_IsSprinting = true;
        }
        else {
            m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
            m_IsSprinting = false;
        }
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_IsRunning = true;
    }
    if(Input::GetInstance()->GetLKM() && !frame && m_IsGrounded && !m_IsDefending && !s_Defendingfl && !s_IsTakenHit && !m_IsDead){
        m_RigidBody->UnSetForce();
        frame = 11;
        m_IsAttacking = true;
    }
    std::cout << dt << std::endl;
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) && m_IsGrounded && !frame && !m_IsAttacking && !m_IsDefending && !s_Defendingfl && !s_IsTakenHit && !m_IsDead){
        m_IsJumping = true;
        m_IsGrounded = false;
        m_RigidBody->ApplyForceY(UPWARD*m_JumpForce);
    }
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) && m_IsJumping && m_JumpTime > 0 && !CollisionHandler::GetInstance()->MapCollision(s_Collider->Get(), 2) && !m_IsDefending && !s_Defendingfl && !s_IsTakenHit && !m_IsDead){
        m_JumpTime -= dt;
        m_RigidBody->ApplyForceY(UPWARD*m_JumpForce);
    }
    else{
        m_IsJumping = false;
        m_JumpTime = JUMP_TIME;
    }
    if(m_IsDead){
        m_DeathTime -= dt;
    }
    if (m_DeathTime <= 0)
        return 3;
    if(s_IsTakenHit) {
        m_TakeHitTime -= dt;
        m_RigidBody->UnSetForce();
        if(m_TakeHitTime <= 0) {
            s_IsTakenHit = false;
            m_TakeHitTime = TAKE_HIT_TIME;
            m_HP--;
            Sound::GetInstance()->PlayEffect("dmg");
            if(m_HP == 0)
                m_IsDead = true;
        }
    }
    if(Input::GetInstance()->GetLKM() && !m_IsGrounded){
        m_IsAirAttacking = true;
    }
    if (Input::GetInstance()->GetPKM() && m_IsGrounded){
        m_RigidBody->UnSetForce();
        m_IsDefending = true;
    }
    if (m_IsGrounded)
        m_IsAirAttacking = false;
    if(m_RigidBody->Velocity().Y > 0 && !m_IsGrounded)
        m_IsFalling = true;
    else
        m_IsFalling = false;
    if(m_IsAttacking && m_AttackTime > 0)
        m_AttackTime -= dt;
    else{
        m_IsAttacking = false;
        m_AttackTime = ATTACK_TIME;
    }
    if(m_IsDefending){
        m_AfterDefTime = AFTER_DEF;
        if(m_PrepareDefTime > 0){
            m_PrepareDefTime -= dt;
            m_PrepDef = true;
        }
        else {
            s_Defendingfl = true;
            s_DefSide = m_Flip;
        }
    }
    if(!m_IsDefending && m_PrepareDefTime <= 0){
        m_PrepareDefTime = PREPARE_TO_DEF;
        s_Defendingfl = false;
    }
    if(!m_IsDefending && m_AfterDefTime > 0){
        m_AfterDefTime -= dt;
        m_AfterDef = true;
    }
    else if(m_AfterDefTime <= 0)
        m_AfterDef = false;
    if(((m_IsAttacking && m_AttackTime < ATTACK_TIME / 1.5) || m_IsAirAttacking) && m_Flip == SDL_FLIP_NONE)
        s_AttackCollision->Set(m_Transform->X+16+130, m_Transform->Y+67, 69, 69);
    else
        if(((m_IsAttacking && m_AttackTime < ATTACK_TIME / 1.5) || m_IsAirAttacking) && m_Flip ==  SDL_FLIP_HORIZONTAL)
            s_AttackCollision->Set(m_Transform->X-16+95, m_Transform->Y+67, 69, 69);
        else
            s_AttackCollision->UnSet();
    m_RigidBody->Update(dt);
    m_LastSafePosition.X = m_Transform->X;
    m_Transform->X += m_RigidBody->Position().X;
    s_Collider->Set(m_Transform->X, m_Transform->Y, 30, 60);

    if(CollisionHandler::GetInstance()->MapCollision(s_Collider->Get(), 1) || CollisionHandler::GetInstance()->MapCollision(s_Collider->Get(), 2)) {
        if (m_IsJumping) {
            m_JumpTime -= dt;
        }
        m_Transform->X = m_LastSafePosition.X;
    }
    if(abs(m_Transform->X - Engine::GetInstance()->GetMap()->GetExitX()) < 5 && abs(m_Transform->Y - Engine::GetInstance()->GetMap()->GetExitY()) < 5)
        return 2;
    m_RigidBody->Update(dt);
    m_LastSafePosition.Y = m_Transform->Y;
    m_Transform->Y += m_RigidBody->Position().Y;
    s_Collider->Set(m_Transform->X, m_Transform->Y, 30, 60);
    if(CollisionHandler::GetInstance()->MapCollision(s_Collider->Get(), 1)){
        if (m_IsJumping){
            m_IsGrounded = false;
            m_IsJumping = false;
        }
        else{
            m_IsGrounded = true;
            m_Transform->Y = m_LastSafePosition.Y;
        }
    }
    else{
        m_IsGrounded = false;
    }

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    AnimationState();
    m_Animation->Update();
    return 0;
}

void Warrior::Clean() {
    //TextureManager::GetInstance()->Drop(m_TextureID);
    delete m_RigidBody;
    delete s_Collider;
    delete m_Animation;
}

void Warrior::AnimationState() {
    m_Animation->SetProps("player", 0, 8, 100);
    if(m_IsRunning){
        if(m_IsSprinting)
            m_Animation->SetProps("player", 1, 8, 70);
        else
            m_Animation->SetProps("player", 1, 8, 100);
        }
    if(m_IsJumping) {
        if (m_IsAirAttacking)
            m_Animation->SetProps("player", 5, 8, 130);
        else
            m_Animation->SetProps("player", 2, 3, 100);
    }
    if(m_IsFalling) {
        if (m_IsAirAttacking)
            m_Animation->SetProps("player", 5, 8, 130);
        else
            m_Animation->SetProps("player", 3, 3, 100);
    }
    if(m_IsAttacking)
        m_Animation->SetProps("player", 7, 11, 100);
    if(m_IsDefending) {
        if (m_PrepDef)
            m_Animation->SetProps("player_def1", 0, 3, 150);
        if (s_Defendingfl)
            m_Animation->SetProps("player_def2", 0, 1, 100);
    }
    else
        if(m_AfterDef)
            m_Animation->SetProps("player_def3", 0, 3, 50);
    if (s_IsTakenHit)
        m_Animation->SetProps("player", 12, 6, 100);
    if(m_IsDead)
        m_Animation->SetProps("player", 13, 13, 100);
}