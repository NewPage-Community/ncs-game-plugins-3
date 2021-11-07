#include <ncs>
#include <ncs/account>
#include <ncs/cookie>
#include <ncs/game>
#include <sdkhooks>
#include <sdktools>

#undef REQUIRE_PLUGIN
#include <ncs/chat>
#include <zombiereloaded>
#define REQUIRE_PLUGIN

ConVar cv_skin_only_team;

#define P_NAME P_PRE ... " - Skin"
#define P_DESC "Skin management plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "skin/skin"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitAPI();
    InitNative();

    // lib
    RegPluginLibrary("NCS-Skin");

    MarkNativeAsOptional("ZR_IsClientZombie");

    return APLRes_Success;
}

public void OnAllPluginsLoaded()
{
    InitLibrary();
}

public void OnPluginStart()
{
    InitCmd();
    InitSkin();
    HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
    HookEvent("player_death", Event_PlayerDeath_Pre, EventHookMode_Pre);

    cv_skin_only_team = CreateConVar("skin_only_team", "0", "", 0, true, 0.0, true, 3.0);
}

public void OnPluginEnd()
{
    CloseAPI();
}

public void OnMapStart()
{
    LoadSkins();
}

public Action Event_PlayerSpawn(Event event, const char[] name1, bool dontBroadcast)
{
    int client = GetClientOfUserId(GetEventInt(event, "userid"));
    if (client < 1 || client > MaxClients || !IsClientInGame(client) || IsFakeClient(client))
        return Plugin_Continue;

    Model_PlayerSpawn(client);
    Preview_PlayerSpawn(client);

    return Plugin_Continue;
}

public void NCS_Cookie_OnUserCached(int client)
{
    GetUsedSkin(client);
}

public Action Event_PlayerDeath_Pre(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
    if (IsFakeClient(client))
        return Plugin_Continue;
    RequestFrame(Broadcast_DeathSound, client);
    return Plugin_Continue;
}