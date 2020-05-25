NCS_Account_OnLoaded

// Call when server on initialized
forward void NCS_Account_OnLoaded(int client, int uid);

// NCS_Account_GetUID get uid
// return 0 = invalid
native int NCS_Account_GetUID(int client);

public SharedPlugin __pl_ncs_account = 
{
    name = "NCS-Account",
    file = "np-account.smx",
    required = 1,
};