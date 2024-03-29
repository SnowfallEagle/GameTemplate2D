#pragma once

#include "Game/Actor.h"

class Dialog final : public Entity
{
private:
    static constexpr i32f DIALOG_STRING_WIDTH  = 20;
    static constexpr i32f DIALOG_STRING_HEIGHT = 5;
    static constexpr i32f DIALOG_STRSIZE       = DIALOG_STRING_WIDTH * DIALOG_STRING_HEIGHT;
    static constexpr i32f DIALOG_BUFSIZE       = DIALOG_STRSIZE + 1;

public:
    Actor* m_pAttached;
    f32 m_time;

private:
    b32 m_bRunning;
    char m_text[DIALOG_BUFSIZE];

public:
    virtual void Init(const Vector2& vPosition, s32 width, s32 height, const Texture* pTexture) override;
    virtual void Update(f32 dtTime) override;
    virtual void Draw() override;

    void Run();
    forceinline b32 Running() const { return m_bRunning; }

    forceinline void Attach(Actor* pActor) { m_pAttached = pActor; }
    forceinline void SetTime(f32 time) { m_time = time; }
    void SetText(const char* text);

private:
    void HandlePosition();

    i32f WordLength(const char* text);
    const char* NextWord(const char* text);
};
