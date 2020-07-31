void InitNative()
{
    CreateNative("NCS_Money_Give", Native_GiveMoney);
}

public int Native_GiveMoney(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    int money = GetNativeCell(2);
    int length;
    GetNativeStringLength(3, length);
    char[] reason = new char[length+1];
    GetNativeString(3, reason, length+1);
    
    GiveMoney(client, money, reason);
    return 1;
}