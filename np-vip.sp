#include <ncs>
#include <ncs/account>
#include <ncs/cookie>
#include <adminmenu>

#undef REQUIRE_PLUGIN
#include <ncs/chat>
#define REQUIRE_PLUGIN

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
	LoadTranslations("common.phrases");
	LoadTranslations("basevotes.phrases");
	LoadTranslations("plugin.basecommands");
	
	InitAPI();
	InitCmd();
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

public void NCS_Cookie_OnUserCached(int client)
{
    NameColor_UserInit(client);
}