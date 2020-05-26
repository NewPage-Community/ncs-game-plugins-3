void RegNative()
{
	CreateNative("NCS_Account_GetUID", Native_GetUID);
	CreateNative("NCS_Account_ChangeName", Native_ChangeName);
}

public int Native_GetUID(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	return g_Accounts[client].Valid() ? g_Accounts[client].uid : 0;
}

public int Native_ChangeName(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	char name[MAX_NAME_LENGTH];
	GetNativeString(2, name, MAX_NAME_LENGTH);

	ChangeName(client, name);
	return 1;
}