#if defined _NCS_Account_
    #endinput
#endif
#define _NCS_Account_

#define MAX_UID_LENGTH 20

// Call when server on initialized
forward void NCS_Account_OnUserLoaded(int client, const char[] uid);
// Safety change name event
forward void NCS_Account_OnChangeName(int client, const char[] newname);
// Call when player disconnect (safe to get uid)
forward void NCS_Account_OnUserDisconnect(int client, const char[] uid);

// NCS_Account_GetUID get uid
// return false = invalid
native bool NCS_Account_GetUID(int client, char[] uid, int size);
// NCS_Account_ChangeName safety way to change name
native void NCS_Account_ChangeName(int client, const char[] name);
// NCS_Account_GetName safety way to get name
native void NCS_Account_GetName(int client, char[] buffer, int size);
// NCS_Account_GetClientOfUID faster way to get uid
// not found return 0
native int NCS_Account_GetClientOfUID(const char[] uid);

public SharedPlugin __pl_ncs_account = 
{
    name = "NCS-Account",
    file = "np-account.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_account_SetNTVOptional()
{
    MarkNativeAsOptional("NCS_Account_GetUID");
    MarkNativeAsOptional("NCS_Account_ChangeName");
    MarkNativeAsOptional("NCS_Account_GetName");
    MarkNativeAsOptional("NCS_Account_GetClientOfUID");
}
#endif