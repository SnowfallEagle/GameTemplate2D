/* ====== INCLUDES ====== */
#include <string.h>

#include "GraphicsModule.h"
#include "GTUnit.h"

#include "Console.h"

/* ====== DEFINES ====== */
#define CONSOLE_STRING_WIDTH 80
#define CONSOLE_STRING_HEIGHT 15
#define CONSOLE_BUFSIZE ((CONSOLE_STRING_WIDTH * CONSOLE_STRING_HEIGHT) + 1)

#define CONSOLE_INPUT_INDEX (CONSOLE_STRING_WIDTH * (CONSOLE_STRING_HEIGHT - 1))

#define CONSOLE_UNIT_HEIGHT 2.5f

static const char s_consolePrompt[] = "> ";

/* ====== VARIABLES ====== */
Console g_console;

/* ====== METHODS ====== */
b32f Console::StartUp()
{
    // Allocate and init buffer
    m_buffer = new u8[CONSOLE_BUFSIZE];
    Clear();
    Reset();

    // Defaults
    m_bShown = false;

    AddNote(PR_NOTE, "Module started");

    return true;
}

void Console::ShutDown()
{
    delete[] m_buffer;
}

void Console::Render() const
{
    // Draw console's background
    SDL_Rect dest = { 0, 0, g_graphicsModule.GetScreenWidth(), g_graphicsModule.GetScreenHeight() / 2 };
    g_graphicsModule.SetColor(0x00, 0x00, 0x00, 0x10);
    g_graphicsModule.FillRect(&dest);

    // Draw text
    g_graphicsModule.DrawText(&dest, GraphicsModule::s_pConsoleFont, (const char*)m_buffer, { 255, 255, 255, 255 });
}

void Console::Print(const char* text)
{
    for (i32f i = 0; i < strlen(text); ++i)
    {
        // TODO(sean)
    }
}

void Console::Input(i32f ch)
{
    // Erase on backspace
    if (ch == SDLK_BACKSPACE)
    {
        if (m_currentInput > CONSOLE_INPUT_INDEX + strlen(s_consolePrompt))
        {
            --m_currentInput;
            m_buffer[m_currentInput] = ' ';
        }
        return;
    }

    // Clear input on escape
    if (ch == SDLK_ESCAPE)
    {
        Reset();
        return;
    }

    // Return if there no space for new character
    if (m_currentInput >= CONSOLE_BUFSIZE - 1)
        return;

    // Place character
    m_buffer[m_currentInput] = (u8)ch;
    ++m_currentInput;
}

void Console::Interpret()
{
    // Additional work on buffer...
    // g_scriptModule.ConsoleInterpret(&m_buffer[...])

    // Reset console's input
    Reset();
}

void Console::Clear()
{
    // Clear buffer
    memset(m_buffer, ' ', CONSOLE_BUFSIZE-1);
    m_buffer[CONSOLE_BUFSIZE - 1] = 0;

    // Reset row
    m_currentRow = 0;
}

void Console::Reset()
{
    m_currentInput = CONSOLE_INPUT_INDEX + (s32)strlen(s_consolePrompt);
    memset(&m_buffer[CONSOLE_INPUT_INDEX], ' ', CONSOLE_STRING_WIDTH);
    memcpy(&m_buffer[CONSOLE_INPUT_INDEX], s_consolePrompt, strlen(s_consolePrompt));
}