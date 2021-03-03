#include <ncs>

#include <ncs/account>

#include <ncs/chat>

#define P_NAME P_PRE ... " - Pass"
#define P_DESC "Pass API provider plugin"

ConVar cv_pass_starttime;
ConVar cv_pass_endtime;

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "pass/pass"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitNative();
    
    // lib
    RegPluginLibrary("NCS-Pass");

    return APLRes_Success;
}

public void OnPluginStart()
{
    InitAPI();
    InitCache();
    InitCmd();

    cv_pass_starttime = CreateConVar("pass_starttime", "0", "", 0, true, 0.0);
    cv_pass_endtime = CreateConVar("pass_endtime", "0", "", 0, true, 0.0);
}

public void OnPluginEnd()
{
    CloseAPI();
    CloseCache();
}

public void OnMapStart()
{
    LoadRewards();
}

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
    InitPlayer(client, uid);
}

public void OnClientDisconnect(int client)
{
    PlayerDisconnect(client);
}