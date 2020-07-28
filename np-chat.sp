// for dev
//#define DEV

#include <ncs>

#define P_NAME P_PRE ... " - Chat"
#define P_DESC "Chat management plugin"

public Plugin myinfo = 
{
    name        = P_NAME,
    author      = P_AUTHOR,
    description = P_DESC,
    version     = P_VERSION,
    url         = P_URLS
};

// Module
#include "chat/chat"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    InitNative();

    // lib
    RegPluginLibrary("NCS-Chat");

    return APLRes_Success;
}

public void OnConfigsExecuted()
{
    ChatConfig();
}

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
	player[client].index = client;
}

public void OnClientDisconnect(int client)
{
	player[client].Clear();
}