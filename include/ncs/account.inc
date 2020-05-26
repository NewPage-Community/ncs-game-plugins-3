#if defined _NCS_Account_
	#endinput
#endif
#define _NCS_Account_

// Call when server on initialized
forward void NCS_Account_OnLoaded(int client, int uid);
// Safety change name event
forward void NCS_Account_OnChangeName(int client, const char[] newname);

// NCS_Account_GetUID get uid
// return 0 = invalid
native int NCS_Account_GetUID(int client);
// NCS_Account_ChangeName safety way to change name
native void NCS_Account_ChangeName(int client, const char[] name);

public SharedPlugin __pl_ncs_account = 
{
    name = "NCS-Account",
    file = "np-account.smx",
    required = 1,
};