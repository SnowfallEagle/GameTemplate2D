#include "Graphics/GraphicsModule.h"
#include "Game/Entity.h"

void Entity::Init(const Vector2& vPosition, s32 width, s32 height, const Texture* pTexture)
{
    m_type = ENTITY_TYPE_ENTITY;

    m_vPosition = vPosition;
    m_vVelocity = { 0.0f, 0.0f };

    m_width = width;
    m_height = height;

    m_angle = 0.0f;
    m_flip = SDL_FLIP_NONE;

    f32 fWidthDiv2 = (f32)width/2.0f - g_graphicsModule.UnitsToPixelsX(1.0f);
    f32 fHeightDiv2 = (f32)height/2.0f - g_graphicsModule.UnitsToPixelsY(1.0f);
    m_hitBox = { -fWidthDiv2, -fHeightDiv2, fWidthDiv2, fHeightDiv2 };
    m_bCollidable = true;

    m_animFrame = 0;
    m_animElapsed = 0.0f;
    m_pAnim = nullptr;

    m_renderMode = RENDER_MODE_DYNAMIC;
    m_zIndex = 0;
    m_bHUD = false;
    m_pTexture = pTexture;
}

void Entity::Draw()
{
    SDL_Rect dstRect = {
        (s32)(m_vPosition.x + 0.5f) - m_width/2, (s32)(m_vPosition.y+ 0.5f) - m_height/2,
        m_width, m_height
    };

    if (m_pAnim)
    {
        g_graphicsModule.DrawFrame(m_renderMode, m_zIndex, m_bHUD, dstRect, m_pTexture, m_pAnim->row, m_animFrame, m_angle, m_flip);
    }
    else
    {
        g_graphicsModule.DrawFrame(m_renderMode, m_zIndex, m_bHUD, dstRect, m_pTexture, 0, m_animFrame, m_angle, m_flip);
    }
}
