#include <ncs>

#include <ncs/account>
#include <ncs/cookie>

#include <sdkhooks>
#include <sdktools>

#undef REQUIRE_PLUGIN
#include <ncs/chat>
#include <zombiereloaded>

#define P_NAME P_PRE ... " - Aura"
#define P_DESC "Aura management plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "aura/aura"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitNative();

    // lib
    RegPluginLibrary("NCS-Aura");

    MarkNativeAsOptional("ZR_IsClientZombie");

    return APLRes_Success;
}

public void OnAllPluginsLoaded()
{
    InitLibrary();
}

public void OnPluginStart()
{
    InitAPI();
    InitCmd();
    HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
    HookEvent("player_death", Event_PlayerDeath, EventHookMode_Post);
}

public void OnPluginEnd()
{
    CloseAPI();
}

public void OnMapStart()
{
    InitParticle();
    LoadAura();   
}

public void NCS_Cookie_OnUserCached(int client)
{
    GetUsedAura(client);
}

public Action Event_PlayerSpawn(Event event, const char[] name1, bool dontBroadcast)
{
    int client = GetClientOfUserId(GetEventInt(event, "userid"));
    if (client < 1 || client > MaxClients || !IsClientInGame(client) || GetClientTeam(client) < 2)
        return Plugin_Continue;

    SetClientAura(client);

    return Plugin_Continue;
}

public Action Event_PlayerDeath(Event event, const char[] name1, bool dontBroadcast)
{
    int client = GetClientOfUserId(GetEventInt(event, "userid"));
    if (client < 1 || client > MaxClients || !IsClientInGame(client) || GetClientTeam(client) < 2)
        return Plugin_Continue;

    RemoveClientAura(client);

    return Plugin_Continue;
}