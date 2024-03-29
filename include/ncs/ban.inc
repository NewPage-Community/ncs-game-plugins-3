#if defined _NCS_Ban_
	#endinput
#endif
#define _NCS_Ban_

// NCS_Ban_BanClient
native void NCS_Ban_BanClient(int client, int type, int length, const char[] reason);

public SharedPlugin __pl_ncs_ban = 
{
	name = "NCS-Bans",
	file = "np-bans.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_ban_SetNTVOptional()
{
	MarkNativeAsOptional("NCS_Ban_BanClient");
}
#endif