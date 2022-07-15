#include <ncs>

#define P_NAME P_PRE ... " - Account"
#define P_DESC "Account management plugin"

public Plugin myinfo = 
{
	name		= P_NAME,
	author	  = P_AUTHOR,
	description = P_DESC,
	version	 = P_VERSION,
	url		 = P_URLS
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

public void OnClientConnected()(int client)
{
	if(IsFakeClient(client) || IsClientSourceTV(client))
		return;

	// Init
	ResetIgnoreChangeName(client);

	char steamid[32];
	if (!GetClientAuthId(client, AuthId_SteamID64, steamid, sizeof(steamid), false))
	{
		NCS_LogError("Account", "OnClientConnected", "Can not verify client SteamID64 -> \"%L\"", client);
		ClientCommand("retry");
		return;
	}
	
	ReqAccountUID(steamid);

	// Check loaded
	StartCheck(client);
}

public void OnClientDisconnect(int client)
{
	UserDisconnectForward(client, g_Accounts[client].uid);
	RemoveClient(client);
	g_Accounts[client].Clean();
}