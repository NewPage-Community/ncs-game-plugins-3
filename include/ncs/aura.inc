#if defined _NCS_Aura_
	#endinput
#endif
#define _NCS_Aura_

native void NCS_Aura_Preview(int client, int id);

public SharedPlugin __pl_ncs_aura = 
{
	name = "NCS-Aura",
	file = "np-aura.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_aura_SetNTVOptional()
{
	MarkNativeAsOptional("NCS_Aura_Preview");
}
#endif