/* ====== INCLUDES ====== */
#include "InputModule.h"
#include "SoundModule.h"

#include "Game.h"

/* ====== VARIABLES ====== */
Game g_game;

static s32 soundID; // DEBUG(sean) remove this

/* ====== METHODS====== */
b32 Game::StartUp()
{
    // Defaults
    m_bRunning = true;

    // DEBUG(sean)
    soundID = g_soundModule.LoadWAV("123.wav");
    g_soundModule.PlaySound(soundID);

    AddNote(PR_NOTE, "Module started");

    return true;
}

void Game::ShutDown()
{
    // DEBUG(sean)
    g_soundModule.UnloadSound(soundID);

    AddNote(PR_NOTE, "Module shut down");
}

void Game::Update(f32 dtTime)
{
    m_dtTime = dtTime;

    if (g_inputModule.IsKeyDown(SDLK_ESCAPE))
        m_bRunning = false;
}

void Game::Render() const
{
    /*
    u8* screen;
    s32 pitch;

    { // Render game objects
        g_graphicsModule.ClearScreen();
        if (!g_graphicsModule.LockBack(screen, pitch))
            return;

        g_graphicsModule.DrawQuad2(screen, pitch, 50, 300, 200, 250, 300, 400, 300, 400, 200);

        g_graphicsModule.UnlockBack();
    }

    { // Render debug stuff
        g_graphicsModule.DrawText_GDI(0, 0, 0, 255, 0, "FPS: %f", 1000.0f/m_dtTime);
        g_graphicsModule.DrawText_GDI(0, 100, 0, 255, 0, "X:%d Y:%d Z:%d", g_inputModule.GetMouseRelX(), g_inputModule.GetMouseRelY(), g_inputModule.GetMouseRelZ());
    }

    // Flip screen
    g_graphicsModule.Flip();
    */
}
