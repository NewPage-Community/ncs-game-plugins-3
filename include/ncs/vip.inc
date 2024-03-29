#if defined _NCS_VIP_
	#endinput
#endif
#define _NCS_VIP_

native bool NCS_VIP_IsVIP(int client);
native int NCS_VIP_Level(int client);
native void NCS_VIP_AddPoint(int client, int addPoint);
native void NCS_VIP_Renewal(int client, int length);
native void NCS_VIP_ExpireTime(int client, char[] buffer, int maxlen);
native void NCS_VIP_ReloadClient(int client);

public SharedPlugin __pl_ncs_vip = 
{
	name = "NCS-VIP",
	file = "np-vip.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_vip_SetNTVOptional()
{
	MarkNativeAsOptional("NCS_VIP_IsVIP");
	MarkNativeAsOptional("NCS_VIP_Level");
	MarkNativeAsOptional("NCS_VIP_AddPoint");
	MarkNativeAsOptional("NCS_VIP_Renewal");
	MarkNativeAsOptional("NCS_VIP_ExpireTime");
	MarkNativeAsOptional("NCS_VIP_ReloadClient");
}
#endif