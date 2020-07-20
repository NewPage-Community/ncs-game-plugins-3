#pragma semicolon 1

#include <sourcemod>
#include <ncs>
#include <ncs/account>
#include "title/title"


#define P_NAME P_PRE ... " - Title"
#define P_DESC "Title management plugin"

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
	RegPluginLibrary("NCS-Title");

	return APLRes_Success;
}

public void OnPluginStart()
{
	InitAPI();
	RegConsoleCmd("sm_prefix", Command_Prefix, "sm_prefix <prefix>");
}

public void OnPluginEnd()
{
	CloseAPI();
}

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
	RequreTitle(client, uid);
}

public void NCS_Account_OnChangeName(int client, const char[] newname)
{
	TitleBeSet(client, customTitle[client].type);
	return;
}
