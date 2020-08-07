// for dev
//#define DEV

#include <ncs>

#define REQUIRE_PLUGIN
#include <ncs/account>
#undef REQUIRE_PLUGIN

#include <ncs/skin>
#include <ncs/trail>
#include <ncs/aura>
#include <ncs/chat>

#define P_NAME P_PRE ... " - Store"
#define P_DESC "Store plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
};

// Module
#include "store/store"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	// lib
	RegPluginLibrary("NCS-Store");

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
