// for dev
//#define DEV

#include <ncs>
#include <ncs/account>

#define P_NAME P_PRE ... " - VIP"
#define P_DESC "VIP API provider plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
};

// Module
#include "vip/vip"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	// core
	InitNative();

	// lib
	RegPluginLibrary("NCS-VIP");

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

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
	ReqVIPInfo(client, uid);
}

public void OnClientDisconnect(int client)
{
	g_VIPs[client].Clean();
}