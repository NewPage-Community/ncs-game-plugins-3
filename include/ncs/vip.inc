#if defined _NCS_VIP_
	#endinput
#endif
#define _NCS_VIP_

native bool NCS_VIP_IsVIP(int client);
native int NCS_VIP_Level(int client);
native void NCS_VIP_AddPoint(int client, int addPoint);
native void NCS_VIP_Renewal(int client, int length);

public SharedPlugin __pl_ncs_vip = 
{
    name = "NCS-VIP",
    file = "np-vip.smx",
    required = 1,
};