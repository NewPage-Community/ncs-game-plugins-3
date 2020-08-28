// for dev
//#define DEV

#include <ncs>

#include <ncs/account>
#include <ncs/server>

#undef REQUIRE_PLUGIN
#include <ncs/chat>

#define P_NAME P_PRE ... " - Sign"
#define P_DESC "Sign API provider plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
};

// Module
#include "sign/sign"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	// lib
	RegPluginLibrary("NCS-Sign");

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
