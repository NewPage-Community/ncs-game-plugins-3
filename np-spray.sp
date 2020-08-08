// for dev
//#define DEV

#include <ncs>

#define REQUIRE_PLUGIN
#include <ncs/account>
#include <ncs/cookie>
#undef REQUIRE_PLUGIN

#include <ncs/chat>
#include <sdkhooks>
#include <sdktools>

#define P_NAME P_PRE ... " - Spray"
#define P_DESC "Spray management plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "spray/spray"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitNative();

    // lib
    RegPluginLibrary("NCS-Spray");

    return APLRes_Success;
}

public void OnPluginStart()
{
    InitAPI();
    InitCmd();
}

public void OnPluginEnd()
{
    CloseAPI();
}

public void OnMapStart()
{
    LoadSpray();
}

public void NCS_Cookie_OnUserCached(int client)
{
    GetUsedSpray(client);
}

public void OnPlayerRunCmdPost(int client, int buttons, int impulse, const float vel[3], const float angles[3], int weapon, int subtype, int cmdnum, int tickcount, int seed, const int mouse[2])
{
    PlayerOnRunCmd(client, buttons);
}