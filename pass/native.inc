void InitNative()
{
    CreateNative("NCS_Pass_AddPoint", Native_AddPoint);
}

public int Native_AddPoint(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    int point = GetNativeCell(2);
    if (playerPass[client].IsValid())
        playerPass[client].AddPoint(point);
    return 1;
}