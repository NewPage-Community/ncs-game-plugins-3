void RegNative()
{
	CreateNative("NCS_Account_GetUID", Native_GetUID);
	CreateNative("NCS_Account_ChangeName", Native_ChangeName);
}

public int Native_GetUID(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	if (!g_Accounts[client].Valid())
		return 0;
	return SetNativeString(2, g_Accounts[client].uid, GetNativeCell(3));
}

public int Native_ChangeName(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	char name[MAX_NAME_LENGTH];
	GetNativeString(2, name, MAX_NAME_LENGTH);

	ChangeName(client, name);
	return 1;
}