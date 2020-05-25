void RegNative()
{
	CreateNative("NCS_Account_GetUID", Native_GetUID);
}

public int Native_GetUID(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	return g_Accounts[client].Valid() ? g_Accounts[client].uid : 0;
}
