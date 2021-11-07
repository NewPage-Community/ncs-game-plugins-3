#include <ncs>
#include <ncs/account>

#define P_NAME P_PRE ... " - Money"
#define P_DESC "Money API provider plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "money/money"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitAPI();
    InitNative();
    
    // lib
    RegPluginLibrary("NCS-Money");

    return APLRes_Success;
}

public void OnPluginStart()
{
    InitCmd();
}

public void OnPluginEnd()
{
    CloseAPI();
}