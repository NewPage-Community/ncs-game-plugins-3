#include <ncs>
#include <ncs/account>

#define P_NAME P_PRE ... " - Cookie"
#define P_DESC "Cookie API provider plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "cookie/cookie"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitNative();

    // lib
    RegPluginLibrary("NCS-Cookie");

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

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
    LoadUserCookie(client, uid);
}