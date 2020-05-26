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

public SharedPlugin __pl_ncs_server = 
{
    name = "NCS-Server",
    file = "np-server.smx",
    required = 1,
};