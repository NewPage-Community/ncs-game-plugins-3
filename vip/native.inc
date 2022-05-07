void InitNative()
{
	CreateNative("NCS_VIP_IsVIP", Native_IsVIP);
	CreateNative("NCS_VIP_Level", Native_Level);
	CreateNative("NCS_VIP_AddPoint", Native_AddPoint);
	CreateNative("NCS_VIP_Renewal", Native_Renewal);
	CreateNative("NCS_VIP_ExpireTime", Native_ExpireTime);
	CreateNative("NCS_VIP_ReloadClient", Native_ReloadClient);
}

public int Native_IsVIP(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	return g_VIPs[client].IsVIP();
}

public int Native_Level(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	return g_VIPs[client].Level();
}

public int Native_AddPoint(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int addPoint = GetNativeCell(2);
	AddPoint(client, addPoint);
	return 1;
}

public int Native_Renewal(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int length = GetNativeCell(2);
	Renewal(client, length);
	return 1;
}

public int Native_ExpireTime(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int length = GetNativeCell(3);
	return SetNativeString(2, g_VIPs[client].expireDate, length);
}

public int Native_ReloadClient(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
		return 0;
	ReqVIPInfo(client, uid, true);
	return 1;
}