#include <ncs>
#include <ncs/account>
#include <ncs/cookie>
#include <sdkhooks>
#include <sdktools>

#undef REQUIRE_PLUGIN
#include <ncs/chat>
#include <zombiereloaded>
#define REQUIRE_PLUGIN

#define P_NAME P_PRE ... " - Trail"
#define P_DESC "Trail management plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "trail/trail"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitNative();

    // lib
    RegPluginLibrary("NCS-Trail");

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
    LoadTrail();   
}

public void NCS_Cookie_OnUserCached(int client)
{
    GetUsedTrail(client);
}

public Action Event_PlayerSpawn(Event event, const char[] name1, bool dontBroadcast)
{
    int client = GetClientOfUserId(GetEventInt(event, "userid"));
    if (client < 1 || client > MaxClients || !IsClientInGame(client) || GetClientTeam(client) < 2)
        return Plugin_Continue;

    SetClientTrail(client);

    return Plugin_Continue;
}

public Action Event_PlayerDeath(Event event, const char[] name1, bool dontBroadcast)
{
    int client = GetClientOfUserId(GetEventInt(event, "userid"));
    if (client < 1 || client > MaxClients || !IsClientInGame(client) || GetClientTeam(client) < 2)
        return Plugin_Continue;

    RemoveClientTrail(client);

    return Plugin_Continue;
}