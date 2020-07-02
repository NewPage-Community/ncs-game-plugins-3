// for dev
//#define DEV

#include <ncs>

// Module
#include "account/account"

#define P_NAME P_PRE ... " - Account"
#define P_DESC "Account management plugin"

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
	// core
	RegNative();

	// lib
	RegPluginLibrary("NCS-Account");

	return APLRes_Success;
}

public void OnPluginStart()
{
	InitAPI();
	InitCmd();
	InitName();
}

public void OnPluginEnd()
{
	CloseAPI();
}

public void OnClientAuthorized(int client, const char[] auth)
{
	if(strcmp(auth, "BOT") == 0 || IsFakeClient(client) || IsClientSourceTV(client))
		return;

	char steamid[32];
	GetClientAuthId(client, AuthId_SteamID64, steamid, sizeof(steamid));
	ReqAccountUID(steamid);

	// Check loaded
	StartCheck(client);
}

public void OnClientDisconnect(int client)
{
	g_Accounts[client].Clean();
}