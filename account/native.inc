void RegNative()
{
	CreateNative("NCS_Account_GetUID", Native_GetUID);
	CreateNative("NCS_Account_ChangeName", Native_ChangeName);
	CreateNative("NCS_Account_GetName", Native_GetName);
	CreateNative("NCS_Account_GetClientOfUID", Native_GetClientOfUID);
	CreateNative("NCS_Account_InSteamGroup", Native_InSteamGroup);
}

public int Native_GetUID(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	if (!g_Accounts[client].Valid())
		return 0;
	return (SetNativeString(2, g_Accounts[client].uid, GetNativeCell(3)) == 0);
}

public int Native_ChangeName(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	char name[MAX_NAME_LENGTH];
	GetNativeString(2, name, MAX_NAME_LENGTH);

	ChangeName(client, name);
	return 1;
}

public int Native_GetName(Handle plugin, int numParams)
{
	char name[MAX_NAME_LENGTH];
	int client = GetNativeCell(1);
	if (!g_Accounts[client].Valid())
	{
		if (!IsValidClient(client))
			return 0;
		GetClientName(client, name, sizeof(name));
	}
	else
		strcopy(name, sizeof(name), g_Accounts[client].name);
	return (SetNativeString(2, name, GetNativeCell(3)) == 0);
}

public int Native_GetClientOfUID(Handle plugin, int numParams)
{
	static char uid[MAX_UID_LENGTH];
	GetNativeString(1, uid, MAX_UID_LENGTH);
	return FindClient(uid);
}

public int Native_InSteamGroup(Handle plugin, int numParams)
{
	static char gid[MAX_GID_LENGTH];
	GetNativeString(2, gid, sizeof(gid));
	return g_Accounts[GetNativeCell(1)].groups.FindString(gid) != -1;
}