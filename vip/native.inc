void InitNative()
{
    CreateNative("NCS_VIP_IsVIP", Native_IsVIP);
    CreateNative("NCS_VIP_Level", Native_Level);
    CreateNative("NCS_VIP_AddPoint", Native_AddPoint);
    CreateNative("NCS_VIP_Renewal", Native_Renewal);
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