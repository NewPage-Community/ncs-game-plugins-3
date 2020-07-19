#if defined _NCS_Account_
	#endinput
#endif
#define _NCS_Account_

#define MAX_UID_LENGTH 20

// Call when server on initialized
forward void NCS_Account_OnUserLoaded(int client, const char[] uid);
// Safety change name event
forward void NCS_Account_OnChangeName(int client, const char[] newname);

// NCS_Account_GetUID get uid
// return false = invalid
native bool NCS_Account_GetUID(int client, char[] uid, int size);
// NCS_Account_ChangeName safety way to change name
native void NCS_Account_ChangeName(int client, const char[] name);
// NCS_Account_GetName safety way to get name
native void NCS_Account_GetName(int client, char[] buffer, int size);

stock int GetClientFromUID(const char[] uid)
{
    for (int i = 1; i < MAXPLAYERS+1; i++)
    {
        int index = i;
        if (index > 0 && index <= MaxClients && IsClientConnected(index) && !IsFakeClient(index) && !IsClientSourceTV(index))
        {
            char tUID[MAX_UID_LENGTH];
            NCS_Account_GetUID(index, tUID, sizeof(tUID));
            if (StrEqual(tUID, uid))
                return index;
        }
    }
    return 0;
}

public SharedPlugin __pl_ncs_account = 
{
    name = "NCS-Account",
    file = "np-account.smx",
    required = 1,
};