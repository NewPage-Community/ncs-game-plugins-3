void InitNative()
{
    CreateNative("NCS_Ban_BanClient", Native_BanClient);
}

public int Native_BanClient(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    int type = GetNativeCell(2);
    int length = GetNativeCell(3);

    int reasonLength;
    GetNativeStringLength(4, reasonLength);
    char[] reason = new int[reasonLength];
	GetNativeString(4, reason, reasonLength);

    ReqBanClient(client, type, length, reason);
}