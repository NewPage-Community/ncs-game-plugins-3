// for dev
//#define DEV

#include <ncs>
#include <ncs/chat>

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

// Module
#include "account/account"

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
	InitUID();
}

public void OnPluginEnd()
{
	CloseAPI();
}

public void OnClientAuthorized(int client, const char[] auth)
{
	if(strcmp(auth, "BOT") == 0 || IsFakeClient(client) || IsClientSourceTV(client))
		return;

	// Init
	ResetIgnoreChangeName(client);

	char steamid[32];
	if (!GetClientAuthId(client, AuthId_SteamID64, steamid, sizeof(steamid)))
	{
		NCS_LogError("Account", "OnClientAuthorized", "Can not verify client SteamID64 -> \"%L\"", client);
		KickClient(client, "无效SteamID，请重启Steam客户端\nInvalid Steam ID, please restart Steam client");
		return;
	}
	
	ReqAccountUID(steamid);

	// Check loaded
	StartCheck(client);
}

public void OnClientDisconnect(int client)
{
	RemoveClient(client);
	g_Accounts[client].Clean();
}