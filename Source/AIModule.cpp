/* ====== INCLUDES ====== */
#include "DebugLogManager.h"
#include "ScriptModule.h"
#include "Actor.h"

#include "AIModule.h"

/* ====== DEFINES ====== */
#define GT_MAX_STATES 256
#define GT_STATE_STRSIZE 30

/* ====== STRUCTURES ====== */
struct GT_State
{
    char functionName[GT_STATE_STRSIZE];
};

/* ====== VARIABLES ====== */
AIModule g_AIModule;

/* ====== METHODS ====== */
b32 AIModule::StartUp()
{
    m_aStates = new GT_State[GT_MAX_STATES];
    m_usedStates = 0;

    AddNote(PR_NOTE, "Module start up");

    return true;
}

void AIModule::ShutDown()
{
    if (m_aStates)
    {
        delete m_aStates;
        m_aStates = nullptr;
        m_usedStates = 0;
    }

    AddNote(PR_NOTE, "Module shut down");
}

void AIModule::HandleState(Actor* pActor)
{
    if (pActor->GetState())
        g_scriptModule.CallStateFunction(pActor->GetState()->functionName);
}

void AIModule::AddNote(s32 priority, const char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    g_debugLogMgr.VAddNote(CHANNEL_SCRIPT, priority, "AIModule", fmt, vl);
    va_end(vl);
}