#include "GTUnit.h"

#include "Entity.h"

void Entity::Init(const Vector2& vPosition, s32 width, s32 height, const GT_Texture* pTexture)
{
    m_type = ENTITY_TYPE_ENTITY;

    m_vPosition = vPosition;
    m_vVelocity = { 0.0f, 0.0f };

    m_width = width;
    m_height = height;

    m_angle = 0.0f;
    m_flip = SDL_FLIP_NONE;

    f32 fWidthDiv2 = (f32)width/2.0f - GTU::UnitToScreenX(1.0f); 
    f32 fHeightDiv2 = (f32)height/2.0f - GTU::UnitToScreenY(1.0f);
    m_hitBox = { -fWidthDiv2, -fHeightDiv2, fWidthDiv2, fHeightDiv2 };
    m_bCollidable = true;

    m_animFrame = 0;
    m_animElapsed = 0.0f;
    m_pAnim = nullptr;

    m_pTexture = pTexture;
}
