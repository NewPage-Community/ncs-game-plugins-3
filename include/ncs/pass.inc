#if defined _NCS_Pass_
	#endinput
#endif
#define _NCS_Pass_

native void NCS_Pass_AddPoint(int client, int point);

public SharedPlugin __pl_ncs_pass = 
{
    name = "NCS-Pass",
    file = "np-pass.smx",
    required = 1,
};