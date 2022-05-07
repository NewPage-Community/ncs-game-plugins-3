#if defined _NCS_Spray_
	#endinput
#endif
#define _NCS_Spray_

native void NCS_Spray_Preview(int client, int id);

public SharedPlugin __pl_ncs_spray = 
{
	name = "NCS-Spray",
	file = "np-spray.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_spray_SetNTVOptional()
{
	MarkNativeAsOptional("NCS_Spray_Preview");
}
#endif