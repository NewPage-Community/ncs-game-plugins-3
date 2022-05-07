#if defined _NCS_Pass_
	#endinput
#endif
#define _NCS_Pass_

native void NCS_Pass_AddPoint(int client, int point);

public SharedPlugin __pl_ncs_pass = 
{
	name = "NCS-Pass",
	file = "np-pass.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_pass_SetNTVOptional()
{
	MarkNativeAsOptional("NCS_Pass_AddPoint");
}
#endif