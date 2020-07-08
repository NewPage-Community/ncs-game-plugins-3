#if defined _NCS_Ban_
	#endinput
#endif
#define _NCS_Ban_

// NCS_Ban_BanClient
native void NCS_Ban_BanClient(int client, int type, int length, const char[] reason);

public SharedPlugin __pl_ncs_account = 
{
    name = "NCS-Bans",
    file = "np-bans.smx",
    required = 1,
};