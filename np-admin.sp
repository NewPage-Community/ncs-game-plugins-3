// for dev
//#define DEV

#include <ncs>
#include <ncs/account>

#define P_NAME P_PRE ... " - Admin"
#define P_DESC "Admin management plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
};

// Module
#include "admin/admin"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	// lib
	RegPluginLibrary("NCS-Admin");

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
	ReqAdmin(client, uid);
}