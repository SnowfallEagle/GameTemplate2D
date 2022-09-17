/* TODO
 * - More stringent check of lua arguments
 */

/* ====== INCLUDES ====== */
extern "C"
{
#include "lauxlib.h"
#include "lualib.h"
}

#include "GraphicsModule.h"
#include "GTUnit.h"
#include "SoundModule.h"
#include "InputModule.h"
#include "Game.h"
#include "Actor.h"
#include "GotoTask.h"

#include "ScriptModule.h"

/* ====== VARIABLES ====== */
ScriptModule g_scriptModule;

/* ====== METHODS ====== */
b32 ScriptModule::StartUp()
{
    // Init lua
    m_pLoader = luaL_newstate();
    luaL_openlibs(m_pLoader);

    DefineFunctions(m_pLoader);
    DefineSymbols(m_pLoader);

    // Init loader script
    if (!CheckLua(m_pLoader, luaL_dofile(m_pLoader, "Scripts/Loader.lua")))
        return false;

    AddNote(PR_NOTE, "Module started");

    return true;
}

void ScriptModule::ShutDown()
{
    if (m_pMission)
    {
        lua_close(m_pMission);
        m_pMission = nullptr;
    }

    if (m_pLoader)
    {
        lua_close(m_pLoader);
        m_pLoader = nullptr;
    }

    AddNote(PR_NOTE, "Module shut down");
}

void ScriptModule::DefineFunctions(lua_State* L)
{
    /* Log */
    lua_register(L, "GT_LOG", _GT_LOG);

    /* Graphics */
    // Textures
    lua_register(L, "defineTexture", _defineTexture);

    // Camera
    lua_register(L, "attachCamera", _attachCamera);
    lua_register(L, "detachCamera", _detachCamera);
    lua_register(L, "setCameraPosition", _setCameraPosition);
    lua_register(L, "setLevelSize", _setLevelSize);

    /* Sound */
    lua_register(L, "defineSound", _defineSound);
    lua_register(L, "playSound", _playSound);

    /* Music */
    lua_register(L, "defineMusic", _defineMusic);
    lua_register(L, "playMusic", _playMusic);

    /* Input */
    lua_register(L, "isKeyDown", _isKeyDown);
    lua_register(L, "isMouseDown", _isMouseDown);
    lua_register(L, "getMousePosition", _getMousePosition);

    /* AI */
    lua_register(L, "defineState", _defineState);

    /* Game */
    lua_register(L, "stopGame", _stopGame);

    /* World */
    // Background stuff
    lua_register(L, "setBackground", _setBackground);
    lua_register(L, "setParallax", _setParallax);

    // Entities
    lua_register(L, "addEntity", _addEntity);
    lua_register(L, "updateEntity", _updateEntity);
    lua_register(L, "updateAllEntities", _updateAllEntities);

    // Actor
    lua_register(L, "addActor", _addActor);
    lua_register(L, "setActorState", _setActorState);
    lua_register(L, "setActorTask", _setActorTask);
    lua_register(L, "sendActorCmd", _sendActorCmd);
}

void ScriptModule::DefineSymbols(lua_State* L)
{
    /* Log */
    lua_pushinteger(L, PR_NOTE);
    lua_setglobal(L, "PR_NOTE");
    lua_pushinteger(L, PR_WARNING);
    lua_setglobal(L, "PR_WARNING");
    lua_pushinteger(L, PR_ERROR);
    lua_setglobal(L, "PR_ERROR");

    /* Graphics */
    lua_pushinteger(L, UNIT_SCREEN_WIDTH);
    lua_setglobal(L, "SCREEN_WIDTH");
    lua_pushinteger(L, UNIT_SCREEN_HEIGHT);
    lua_setglobal(L, "SCREEN_HEIGHT");

    lua_pushinteger(L, TW_LOCATION);
    lua_setglobal(L, "TW_LOCATION");
    lua_pushinteger(L, TH_LOCATION);
    lua_setglobal(L, "TH_LOCATION");

    lua_pushinteger(L, TW_PARALLAX);
    lua_setglobal(L, "TW_PARALLAX");
    lua_pushinteger(L, TH_PARALLAX);
    lua_setglobal(L, "TH_PARALLAX");

    /* Input */
    // WASD/Arrows
    lua_pushinteger(L, SDLK_w);
    lua_setglobal(L, "GTK_W");
    lua_pushinteger(L, SDLK_a);
    lua_setglobal(L, "GTK_A");
    lua_pushinteger(L, SDLK_s);
    lua_setglobal(L, "GTK_S");
    lua_pushinteger(L, SDLK_d);
    lua_setglobal(L, "GTK_D");

    lua_pushinteger(L, SDLK_UP);
    lua_setglobal(L, "GTK_UP");
    lua_pushinteger(L, SDLK_LEFT);
    lua_setglobal(L, "GTK_LEFT");
    lua_pushinteger(L, SDLK_DOWN);
    lua_setglobal(L, "GTK_DOWN");
    lua_pushinteger(L, SDLK_RIGHT);
    lua_setglobal(L, "GTK_RIGHT");

    // Some special symbols
    lua_pushinteger(L, SDLK_ESCAPE);
    lua_setglobal(L, "GTK_ESCAPE");
    lua_pushinteger(L, SDLK_RETURN);
    lua_setglobal(L, "GTK_RETURN");
    lua_pushinteger(L, SDLK_SPACE);
    lua_setglobal(L, "GTK_SPACE");

    // Mouse
    lua_pushinteger(L, GTM_LEFT);
    lua_setglobal(L, "GTM_LEFT");
    lua_pushinteger(L, GTM_RIGHT);
    lua_setglobal(L, "GTM_RIGHT");
    lua_pushinteger(L, GTM_MIDDLE);
    lua_setglobal(L, "GTM_MIDDLE");

    /* AI */
    lua_pushinteger(L, GTC_MOVE_UP);
    lua_setglobal(L, "GTC_MOVE_UP");
    lua_pushinteger(L, GTC_MOVE_LEFT);
    lua_setglobal(L, "GTC_MOVE_LEFT");
    lua_pushinteger(L, GTC_MOVE_DOWN);
    lua_setglobal(L, "GTC_MOVE_DOWN");
    lua_pushinteger(L, GTC_MOVE_RIGHT);
    lua_setglobal(L, "GTC_MOVE_RIGHT");

    lua_pushinteger(L, GTT_NONE);
    lua_setglobal(L, "GTT_NONE");
    lua_pushinteger(L, GTT_DONE);
    lua_setglobal(L, "GTT_DONE");
    lua_pushinteger(L, GTT_INPROCESS);
    lua_setglobal(L, "GTT_INPROCESS");

    lua_pushinteger(L, GTT_GOTO);
    lua_setglobal(L, "GTT_GOTO");
}

b32 ScriptModule::LoadMission()
{
    // Get getMission()
    lua_getglobal(m_pLoader, "getMission");
    if (!lua_isfunction(m_pLoader, -1))
    {
        lua_pop(m_pLoader, 1); // Pop "getMission"
        return false;
    }

    // Call getMission()
    if (!CheckLua(m_pLoader, lua_pcall(m_pLoader, 0, 1, 0)))
        return false;

    // Check returned variable
    if (!lua_isstring(m_pLoader, -1))
    {
        lua_pop(m_pLoader, 1); // Pop "getMission"
        return false;
    }

    // Create mission lua state
    m_pMission = luaL_newstate();
    luaL_openlibs(m_pMission);

    // Define all engine stuff
    DefineFunctions(m_pMission);
    DefineSymbols(m_pMission);

    // Try to open script
    if (!CheckLua(m_pMission, luaL_dofile(m_pMission, lua_tostring(m_pLoader, -1))))
    {
        lua_pop(m_pLoader, 1); // Pop "getMission"
        return false;
    }

    // Pop "getMission"
    lua_pop(m_pLoader, 1);

    // Get onEnter()
    lua_getglobal(m_pMission, "onEnter");
    if (!lua_isfunction(m_pMission, -1))
    {
        lua_pop(m_pMission, 1); // Pop "onEnter"
        return false;
    }

    // Call onEnter()
    if (!CheckLua(m_pMission, lua_pcall(m_pMission, 0, 0, 0)))
        return false;

    return true;
}

void ScriptModule::UnloadMission()
{
    if (m_pMission)
    {
        lua_close(m_pMission);
        m_pMission = nullptr;
    }
}

void ScriptModule::UpdateMission(f32 dtTime)
{
    // Check if we have onUpdate() function
    lua_getglobal(m_pMission, "onUpdate");
    if (!lua_isfunction(m_pMission, -1))
    {
        AddNote(PR_ERROR, "UpdateMission(): There're no lua <onUpdate()> function");
        lua_pop(m_pMission, 1); // Pop "onUpdate"
        return;
    }

    // Call onUpdate()
    lua_pushnumber(m_pMission, dtTime);
    lua_pcall(m_pMission, 1, 0, 0);
}

void ScriptModule::CallStateFunction(Actor* pActor, const char* functionName)
{
    // Check for null
    if (!functionName)
    {
        AddNote(PR_WARNING, "CallStateFunction: called with null functionName");
        return;
    }

    // Get function
    lua_getglobal(m_pMission, functionName);
    if (!lua_isfunction(m_pMission, -1))
    {
        lua_pop(m_pMission, 1);
        AddNote(PR_WARNING, "CallStateFunction(): there're no function %s", functionName);
        return;
    }

    // Call function
    lua_pushlightuserdata(m_pMission, pActor);
    lua_pcall(m_pMission, 1, 0, 0);
}

void ScriptModule::LuaNote(s32 priority, const char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    g_debugLogMgr.VAddNote(CHANNEL_SCRIPT, priority, "Lua", fmt, vl);
    va_end(vl);
}

b32 ScriptModule::LuaExpect(lua_State* L, const char* funName, s32 expect)
{
    s32 given;
    if ((given = lua_gettop(L)) == expect)
    {
        return true;
    }
    else
    {
        LuaNote(PR_ERROR, "%s(): Expected %d arguments, but %d given", funName, expect, lua_gettop(L));
        return false;
    }
}

b32 ScriptModule::CheckLua(lua_State* L, s32 res)
{
    if (res != LUA_OK)
    {
        AddNote(PR_WARNING, "CheckLua(): %s", lua_tostring(L, -1));
        lua_pop(L, 1); // Pop error
        return false;
    }

    return true;
}

s32 ScriptModule::_GT_LOG(lua_State* L)
{
    if (!LuaExpect(L, "GT_LOG", 2))
        return -1;

    if (lua_isinteger(L, 1) && lua_isstring(L, 2))
        LuaNote((s32)lua_tointeger(L, 1), lua_tostring(L, 2));

    return 0;
}

s32 ScriptModule::_defineTexture(lua_State* L)
{
    if (!LuaExpect(L, "defineTexture", 3))
        return -1;

    lua_pushlightuserdata(L, (void*)g_graphicsModule.DefineTexture(lua_tostring(L, 1),
                                                                   (s32)lua_tointeger(L, 2),
                                                                   (s32)lua_tointeger(L, 3)));

    return 1;
}

s32 ScriptModule::_attachCamera(lua_State* L)
{
    if (!LuaExpect(L, "attachCamera", 1))
        return -1;

    g_graphicsModule.GetCamera().Attach( (Entity*)lua_touserdata(L, 1) );

    return 0;
}

s32 ScriptModule::_detachCamera(lua_State* L)
{
    if (!LuaExpect(L, "detachCamera", 0))
        return -1;

    g_graphicsModule.GetCamera().Detach();

    return 0;
}

s32 ScriptModule::_setCameraPosition(lua_State* L)
{
    if (!LuaExpect(L, "setCameraPosition", 2))
        return -1;

    g_graphicsModule.GetCamera().SetPosition((s32)GTU::UnitToScreenX((f32)lua_tonumber(L, 1)),
                                             (s32)GTU::UnitToScreenY((f32)lua_tonumber(L, 2)));

    return 0;
}

s32 ScriptModule::_setLevelSize(lua_State* L)
{
    if (!LuaExpect(L, "setLevelSize", 2))
        return -1;

    SRect rect = {
        0, // x1
        0, // y1
        (s32)( GTU::UnitToScreenX((f32)lua_tonumber(L, 1)) ) - 1, // x2
        (s32)( GTU::UnitToScreenY((f32)lua_tonumber(L, 2)) ) - 1, // y2
    };

    g_graphicsModule.GetCamera().SetBoundary(rect);

    return 0;
}

s32 ScriptModule::_setBackground(lua_State* L)
{
    if (!LuaExpect(L, "setBackground", 1))
        return -1;

    g_game.GetWorld().SetBackground( (GT_Texture*)lua_touserdata(L, 1) );

    return 0;
}

s32 ScriptModule::_setParallax(lua_State* L)
{
    if (!LuaExpect(L, "setParallax", 1))
        return -1;

    g_game.GetWorld().SetParallax( (GT_Texture*)lua_touserdata(L, 1) );

    return 0;
}

s32 ScriptModule::_defineSound(lua_State* L)
{
    if (!LuaExpect(L, "defineSound", 1))
        return -1;

    lua_pushlightuserdata(L, g_soundModule.DefineWAV(lua_tostring(L, 1)));

    return 1;
}

s32 ScriptModule::_playSound(lua_State* L)
{
    if (!LuaExpect(L, "playSound", 1))
        return -1;

    g_soundModule.PlaySound( (GT_Sound*)lua_touserdata(L, 1) );

    return 0;
}

s32 ScriptModule::_defineMusic(lua_State* L)
{
    if (!LuaExpect(L, "defineMusic", 1))
        return -1;

    lua_pushlightuserdata(L, g_soundModule.DefineMusic(lua_tostring(L, 1)));

    return 1;
}

s32 ScriptModule::_playMusic(lua_State* L)
{
    if (!LuaExpect(L, "playMusic", 1))
        return -1;

    g_soundModule.PlayMusic( (GT_Music*)lua_touserdata(L, 1) );

    return 0;
}

s32 ScriptModule::_isKeyDown(lua_State* L)
{
    if (!LuaExpect(L, "isKeyDown", 1))
        return -1;

    lua_pushboolean(L, g_inputModule.IsKeyDown((SDL_Keycode)lua_tointeger(L, 1)));

    return 1;
}

s32 ScriptModule::_isMouseDown(lua_State* L)
{
    if (!LuaExpect(L, "isMouseDown", 1))
        return -1;

    lua_pushboolean(L, g_inputModule.IsMouseDown((Uint32)lua_tointeger(L, 1)));

    return 1;
}

s32 ScriptModule::_getMousePosition(lua_State* L)
{
    if (!LuaExpect(L, "getMousePosition", 0))
        return -1;

    // Get mouse screen position
    s32 x, y;
    g_inputModule.GetMousePosition(x, y);

    // Return
    lua_pushinteger(L, (lua_Integer)( GTU::ScreenToUnitX((f32)x) ));
    lua_pushinteger(L, (lua_Integer)( GTU::ScreenToUnitY((f32)y) ));

    return 2;
}

s32 ScriptModule::_defineState(lua_State* L)
{
    if (!LuaExpect(L, "defineState", 1))
        return -1;

    lua_pushlightuserdata(L, (void*)g_AIModule.DefineState(lua_tostring(L, 1)));

    return 1;
}

s32 ScriptModule::_stopGame(lua_State* L)
{
    if (!LuaExpect(L, "stopGame", 0))
        return -1;

    g_game.Stop();

    return 0;
}

s32 ScriptModule::_addEntity(lua_State* L)
{
    if (!LuaExpect(L, "addEntity", 5))
        return -1;

    // Init entity
    Entity* pEntity = new Entity();

    Vector2 vPosition = { GTU::UnitToScreenX((f32)lua_tonumber(L, 1)),
                          GTU::UnitToScreenY((f32)lua_tonumber(L, 2)) };
    s32 width  = (s32)( GTU::UnitToScreenX((f32)lua_tonumber(L, 3)) );
    s32 height = (s32)( GTU::UnitToScreenY((f32)lua_tonumber(L, 4)) );
    GT_Texture* pTexture = (GT_Texture*)lua_touserdata(L, 5);

    pEntity->Init(vPosition, width, height, pTexture);

    // Push him to the world
    g_game.GetWorld().AddEntity(pEntity);

    // Return pointer to lua
    lua_pushlightuserdata(L, pEntity);

    return 1;
}

s32 ScriptModule::_updateEntity(lua_State* L)
{
    if (!LuaExpect(L, "updateEntity", 2))
        return -1;

    Entity* pEntity = (Entity*)lua_touserdata(L, 1);
    if (pEntity)
        pEntity->Update((f32)lua_tonumber(L, 2));
    else
        LuaNote(PR_WARNING, "updateEntity(): function called with null entity");

    return 0;
}

s32 ScriptModule::_updateAllEntities(lua_State* L)
{
    if (!LuaExpect(L, "updateAllEntities", 1))
        return -1;

    g_game.GetWorld().UpdateAllEntities((f32)lua_tonumber(L, 1));

    return 0;
}

s32 ScriptModule::_addActor(lua_State* L)
{
    if (!LuaExpect(L, "addActor", 5))
        return -1;

    // Init actor
    Actor* pActor = new Actor();

    Vector2 vPosition = { GTU::UnitToScreenX((f32)lua_tonumber(L, 1)),
                          GTU::UnitToScreenY((f32)lua_tonumber(L, 2)) };
    s32 width  = (s32)( GTU::UnitToScreenX((f32)lua_tonumber(L, 3)) );
    s32 height = (s32)( GTU::UnitToScreenY((f32)lua_tonumber(L, 4)) );
    GT_Texture* pTexture = (GT_Texture*)lua_touserdata(L, 5);

    pActor->Init(vPosition, width, height, pTexture);

    // Push him to the world
    g_game.GetWorld().AddEntity(pActor);

    // Return pointer to lua
    lua_pushlightuserdata(L, pActor);

    return 1;
}

s32 ScriptModule::_sendActorCmd(lua_State* L)
{
    if (lua_gettop(L) < 2)
    {
        LuaNote(PR_ERROR, "sendActorCmd(): expected at least 2 arguments but %d given", lua_gettop(L));
        return -1;
    }

    // Init command
    GT_Command cmd;
    cmd.cmd = (s32)lua_tointeger(L, 2);
    for (s32 i = 3; i <= lua_gettop(L); i++)
    {
        f32 arg = (f32)lua_tonumber(L, i);
        cmd.lstArgument.PushBack(arg);
    }

    // Send command
    static_cast<Actor*>(lua_touserdata(L, 1))->SendCommand(cmd);

    return 0;
}

s32 ScriptModule::_setActorState(lua_State* L)
{
    if (!LuaExpect(L, "setActorState", 2))
        return -1;

    static_cast<Actor*>(lua_touserdata(L, 1))->SetState((GT_State*)lua_touserdata(L, 2));

    return 0;
}

s32 ScriptModule::_setActorTask(lua_State* L)
{
    if (lua_gettop(L) < 2)
    {
        LuaNote(PR_WARNING, "setActorTask(): expected at least 2 arguments but %d given", lua_gettop(L));
        return -1;
    }

    switch (lua_tointeger(L, 2))
    {

    case GTT_GOTO:
    {
        Actor* pActor = (Actor*)lua_touserdata(L, 1);
        Vector2 vDestination = { GTU::UnitToScreenX((f32)lua_tonumber(L, 3)),
                                 GTU::UnitToScreenY((f32)lua_tonumber(L, 4))};
        pActor->SetTask(new GotoTask(pActor, vDestination));
    } break;

    default:
    {
        LuaNote(PR_WARNING, "setActorTask(): undefined task given: %d", lua_tointeger(L, 2));
    } break;

    }

    return 0;
}
