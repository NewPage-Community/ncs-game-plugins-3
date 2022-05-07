#if defined _NCS_Trail_
	#endinput
#endif
#define _NCS_Trail_

native void NCS_Trail_Preview(int client, int id);

public SharedPlugin __pl_ncs_trail = 
{
	name = "NCS-Trail",
	file = "np-trail.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_trail_SetNTVOptional()
{
	MarkNativeAsOptional("NCS_Trail_Preview");
}
#endif