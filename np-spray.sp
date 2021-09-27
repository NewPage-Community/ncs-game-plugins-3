#include <ncs>

#include <ncs/account>
#include <ncs/cookie>

#include <sdkhooks>
#include <sdktools>

#undef REQUIRE_PLUGIN
#include <ncs/chat>

#define P_NAME P_PRE ... " - Spray"
#define P_DESC "Spray management plugin"

#define SPRAY_SOUND "np_sound/spray.wav"

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
    PrecacheSound(SPRAY_SOUND, false);
}

public void NCS_Cookie_OnUserCached(int client)
{
    GetUsedSpray(client);
}

public void OnPlayerRunCmdPost(int client, int buttons, int impulse, const float vel[3], const float angles[3], int weapon, int subtype, int cmdnum, int tickcount, int seed, const int mouse[2])
{
    PlayerOnRunCmd(client, impulse);
}