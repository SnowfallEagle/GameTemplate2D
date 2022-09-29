/* ====== INCLUDES ====== */
#include "Game.h"

#include "Dialog.h"

/* ====== METHODS ====== */
void Dialog::Init(const Vector2& vPosition, s32 width, s32 height, const GT_Texture* pTexture)
{
    Entity::Init(vPosition, width, height, pTexture);
    m_type = ENTITY_TYPE_DIALOG;
    m_bCollidable = false;

    m_renderMode = RENDER_MODE_FOREGROUND;
    m_zIndex = 0; // TODO(sean) Think about this default z-index

    // Defaults
    m_pAttached = nullptr;
    m_time = 0.0f;
    m_bRunning = false;
    m_text[0] = 0;
}

void Dialog::Update(f32 dtTime)
{
    // Return if we're not running
    if (!m_bRunning)
        return;

    // Check time and attached entity
    if (m_time <= 0.0f || !g_game.GetWorld().HasEntity(m_pAttached))
    {
        m_bRunning = false;
        return;
    }

    // Compute position
    if (m_pAttached->IsLookRight())
        m_vPosition.x = m_pAttached->GetPosition().x + m_pAttached->GetHitBox().x2;
    else
        m_vPosition.x = m_pAttached->GetPosition().x + m_pAttached->GetHitBox().x1 - m_width;
    m_vPosition.y = m_pAttached->GetPosition().y + m_pAttached->GetHitBox().y1 - m_height;

    // Handle time
    m_time -= dtTime;
}

void Dialog::Draw()
{
    if (!m_bRunning)
        return;

    // Draw dialog box
    SDL_Rect dest = {
        (s32)m_vPosition.x, (s32)m_vPosition.y,
        m_width, m_height
    };
    // DEBUG(sean)
    //g_graphicsModule.DrawFrame(m_renderMode, m_zIndex, false, dest, m_pTexture, 0, 0);
    g_graphicsModule.SetDrawColor(0x00, 0x00, 0x00, 0xFF);
    g_graphicsModule.FillRect(m_renderMode, m_zIndex, false, dest);

    // Draw text
    dest.h /= DIALOG_STRING_HEIGHT;
    i32f tempIndex = DIALOG_STRING_WIDTH;
    g_graphicsModule.SetDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
    for (i32f i = 0; i < DIALOG_STRING_HEIGHT; ++i)
    {
        // Null terminate string line
        char temp = m_text[tempIndex];
        m_text[tempIndex] = 0;

        // Draw this string line
        g_graphicsModule.DrawText(m_renderMode, m_zIndex + 1, false,
                                  dest, &m_text[tempIndex - DIALOG_STRING_WIDTH],
                                  GraphicsModule::s_pConsoleFont);

        // Recover string
        m_text[tempIndex] = temp;

        // Go to next line
        tempIndex += DIALOG_STRING_WIDTH;
        dest.y += dest.h;
    }
}

void Dialog::SetText(const char* text)
{
    // Copy text
    size_t len = strlen(text);
    if (len > DIALOG_STRSIZE)
        len = DIALOG_STRSIZE;
    memcpy(m_text, text, len);

    // Set spaces and null-terminate
    for (size_t i = len; i < DIALOG_STRSIZE; ++i)
        m_text[i] = ' ';
    m_text[DIALOG_STRSIZE] = 0;
}
