#if defined _NCS_Server_
	#endinput
#endif
#define _NCS_Server_

// Call when server on initialized
forward void NCS_Server_OnLoaded();

// Get Server's ID
// return 0 = invalid
native int NCS_Server_GetID();
// Get Server's ModID
// return 0 = invalid
native int NCS_Server_GetModID();
// Get Server's GameID
// return 0 = invalid
native int NCS_Server_GetGameID();

public SharedPlugin __pl_ncs_server = 
{
	name = "NCS-Server",
	file = "np-server.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_server_SetNTVOptional()
{
	MarkNativeAsOptional("NCS_Server_GetID");
	MarkNativeAsOptional("NCS_Server_GetModID");
	MarkNativeAsOptional("NCS_Server_GetGameID");
}
#endif