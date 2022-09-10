#ifndef INPUTMODULE_H_
#define INPUTMODULE_H_

/* ====== INCLUDES ====== */
#include "SDL.h"

#include "Types.h"
#include "EngineModule.h"

/* ====== DEFINES ====== */

/* ====== STRUCTURES ====== */
enum eMouseButton
{
    IMB_LEFT   = SDL_BUTTON_LMASK,
    IMB_RIGHT  = SDL_BUTTON_RMASK,
    IMB_MIDDLE = SDL_BUTTON_MMASK
};

class InputModule final: public EngineModule
{
    const Uint8* m_keyState;

    Uint32 m_mouseState;
    s32 m_mousePosX, m_mousePosY;
public:
    InputModule();

    b32 StartUp();
    void ShutDown();

    b32 HandleEvents();

    // Keyboard
    b32 IsKeyDown_s(SDL_Scancode scan) const { return m_keyState[scan]; }
    b32 IsKeyDown(SDL_Keycode key) const { return m_keyState[SDL_GetScancodeFromKey(key)]; }

    // Mouse
    b32 IsMouseDown(Uint32 key) const { return m_mouseState & key; }
    s32 GetMousePosX() const { return m_mousePosX; }
    s32 GetMousePosY() const { return m_mousePosY; }
};

extern InputModule g_inputModule;

/* ====== METHODS ====== */
inline InputModule::InputModule() :
    EngineModule("InputModule", CHANNEL_INPUT),
    m_keyState(nullptr), m_mouseState(0),
    m_mousePosX(0), m_mousePosY(0) {}

#endif // INPUTMODULE_H_
