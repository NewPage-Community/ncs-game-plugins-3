#include <ncs>

#include <ncs/account>

#define P_NAME P_PRE ... " - Backpack"
#define P_DESC "Backpack api provider plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
};

// Module
#include "backpack/backpack"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	// core
	InitNative();

	// lib
	RegPluginLibrary("NCS-Backpack");

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