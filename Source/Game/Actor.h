#pragma once

#include "Game/Entity.h"
#include "AI/AIState.h"
#include "AI/AITask.h"
#include "AI/AICommand.h"

enum eActorState
{
    ACTOR_STATE_IDLE = 0,
    ACTOR_STATE_AFTER_ANIMATION,
    ACTOR_STATE_MOVE,
    ACTOR_STATE_ATTACK,
    ACTOR_STATE_DEAD,
    ACTOR_STATE_INCAR,
    ACTOR_STATE_ANIMATE_ONCE,
    ACTOR_STATE_ANIMATE_LOOPED,

    MAX_ACTOR_STATES
};

enum eActorAnimation
{
    ACTOR_ANIMATION_IDLE = 0,
    ACTOR_ANIMATION_HORIZONTAL,
    ACTOR_ANIMATION_TOP,
    ACTOR_ANIMATION_BOTTOM,
    ACTOR_ANIMATION_DEAD,
    ACTOR_ANIMATION_INCAR,

    MAX_ACTOR_ANIMATIONS
};

enum eActorTeam
{
    ACTOR_TEAM_DEFAULT = 0,
};

class Weapon;
struct Sound;

class Actor final : public Entity
{
private:
    static constexpr f32 ACTOR_DEFAULT_HEALTH = 100.0f;
    static constexpr f32 ACTOR_DEFAULT_UNIT_SPEED_X = 0.035f;
    static constexpr f32 ACTOR_DEFAULT_UNIT_SPEED_Y = 0.01f;
    static constexpr f32 ACTOR_DEFAULT_ATTACK_RATE = 1000.0f / 6.0f;

    static constexpr Animation DEFAULT_ACTOR_ANIMS[] =
    {
        { 0, 2, 1000.0f / 1.0f },  // Idle
        { 1, 5, 1000.0f / 12.5f }, // Horizontal locomotion
        { 2, 3, 1000.0f / 6.0f },  // Top locomotion
        { 3, 3, 1000.0f / 6.0f },  // Bottom locomotion
        { 0, 2, 1000.0f / 1.0f },  // Dead
        { 0, 2, 1000.0f / 1.0f },  // In car
    };

public:
    /** Actor stuff */
    s32 m_actorState;
    s32 m_actorTeam;

    Vector2 m_vSpeed;
    f32 m_health;

    b32 m_bGodMode;
    b32 m_bLookRight;

    Sound* m_pDeathSound;

    f32 m_attackRate;
    const Weapon* m_pWeapon;

private:
    /** AI stuff */
    AIState m_state;
    TList<AITask*> m_lstTask;
    TList<s32> m_lstCommand;

public:
    /** Animations */
    const Animation* m_aActorAnims[MAX_ACTOR_ANIMATIONS];

public:
    virtual void Init(const Vector2& vPosition, s32 width, s32 height, const Texture* pTexture) override;
    virtual void Clean() override;
    virtual void Update(f32 dtTime) override;

    void AddHealth(f32 diff);

    forceinline void SetState(const char* functionName) { m_state.SetFunctionName(functionName); }
    forceinline const AIState& GetState() const { return m_state; }

    void PushTask(AITask* pTask);
    void RemoveTasks();
    forceinline const AITask* GetCurrentTask() { return m_lstTask.IsEmpty() ? nullptr : m_lstTask.Front(); }

    forceinline void PushCommand(s32 enumCmd) { m_lstCommand.Push(enumCmd); }

private:
    b32 HandleDeath();
    void HandleActorState(f32 dtTime);

    forceinline void HandleAIState() { m_state.Handle(); }
    void HandleAITasks();
    void HandleAICommand(f32 dtTime);

    forceinline void CommandIdle() { m_actorState = ACTOR_STATE_IDLE; }
    forceinline void CommandTurnLeft() { m_bLookRight = false; }
    forceinline void CommandTurnRight() { m_bLookRight = true; }
    void CommandMove(s32 cmd, f32 dtTime);
    void CommandAttack();

    void HandleAnimation(f32 dtTime);

    void AnimateIdle();
    void AnimateAfterAnimation();
    void AnimateMove();
    void AnimateAttack();
    b32 AnimateDead();
    void AnimateInCar();
};
