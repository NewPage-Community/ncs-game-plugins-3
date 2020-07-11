#if defined _NCS_Sign_
	#endinput
#endif
#define _NCS_Sign_

forward void NCS_Sign_OnUserSigned(int client);

public SharedPlugin __pl_ncs_sign = 
{
    name = "NCS-Sign",
    file = "np-sign.smx",
    required = 1,
};