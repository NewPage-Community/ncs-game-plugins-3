// for dev
#define DEV

#include <ncs>
#include <ncs/account>

#define P_NAME P_PRE ... " - Pass"
#define P_DESC "Pass API provider plugin"

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
}

public void OnPluginEnd()
{
    CloseAPI();
    CloseCache();
}

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
    InitPlayer(client, uid);
}

public void OnClientDisconnect(int client)
{
    PlayerDisconnect(client);
}