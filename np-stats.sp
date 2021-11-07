#include <ncs>
#include <ncs/account>
#include <ncs/stats>

#define P_NAME P_PRE ... " - Stats"
#define P_DESC "Stats api provider plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "stats/stats"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitNative();

    // lib
    RegPluginLibrary("NCS-Stats");

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