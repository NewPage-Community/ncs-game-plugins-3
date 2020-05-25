// for dev
//#define DEV

#include <ncs>

// Module
#include "server/server"

#define P_NAME P_PRE ... " - Server"
#define P_DESC "Server management plugin"

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
	// core
	RegNative();

	// lib
	RegPluginLibrary("NCS-Server");

	return APLRes_Success;
}

public void OnPluginStart()
{
	InitAPI();
	RegCmd();
	RegForward();
}

public void OnMapStart()
{
	ReqServerInfo();
}