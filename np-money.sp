#include <ncs>
#include <ncs/account>

#undef REQUIRE_PLUGIN
#include <ncs/chat>
#define REQUIRE_PLUGIN

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
    InitNative();
    
    // lib
    RegPluginLibrary("NCS-Money");

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