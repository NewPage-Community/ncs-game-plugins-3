#include <ncs>

#include <ncs/account>

#include <ncs/chat>

#define P_NAME P_PRE ... " - Bans"
#define P_DESC "Bans management plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
};

// Module
#include "ban/ban"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	InitNative();

	// lib
	RegPluginLibrary("NCS-Bans");

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
	// Check ban information when player loaded
	ReqBanCheck(client, uid);
}