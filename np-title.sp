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
	RegAdminCmd("sm_prefix", Set_Title, ADMFLAG_SLAY, "sm_prefix <prefix>");
}

public void OnPluginEnd()
{
	CloseAPI();
}

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
	RequreTitle(client, uid);
}