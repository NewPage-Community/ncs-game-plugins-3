// for dev
//#define DEV

#include <ncs>
#include <ncs/account>

// Module
#include "bans/ban"

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

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	// lib
	RegPluginLibrary("NCS-Bans");

	return APLRes_Success;
}

public void OnPluginStart()
{
	InitAPI();
}

public void OnPluginEnd()
{
	CloseAPI();
}

public void NCS_Account_OnUserLoaded(int client, int uid)
{
	ReqBanInfo(client, uid);
}