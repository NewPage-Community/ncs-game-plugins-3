#include <ncs>
#include <ncs/server>

#define P_NAME P_PRE ... " - ConVar"
#define P_DESC "ConVar management plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "convar/convar"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitAPI();
    // lib
    RegPluginLibrary("NCS-ConVar");

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

public void OnConfigsExecuted()
{
    UpdateCVars();
}