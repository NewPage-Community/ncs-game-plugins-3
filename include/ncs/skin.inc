#if defined _NCS_Skin_
	#endinput
#endif
#define _NCS_Skin_

native void NCS_Skin_Preview(int client, int skinID);

public SharedPlugin __pl_ncs_skin = 
{
    name = "NCS-Skin",
    file = "np-skin.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ircrelay_SetNTVOptional()
{
    MarkNativeAsOptional("NCS_Skin_Preview");
}
#endif