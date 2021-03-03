#include <ncs>

#include <ncs/account>
#include <ncs/server>
#include <ncs/cookie>

#undef REQUIRE_PLUGIN
#include <ncs/chat>

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

public void OnPluginStart()
{
    InitCmd();
    InitAPI();
}

public void OnPluginEnd()
{
    CloseAPI();
}

public void OnConfigsExecuted()
{
    ChatConfig();
}

public void OnClientConnected(int client)
{
	player[client].index = client;
}

public void OnClientDisconnect(int client)
{
	player[client].Clear();
}