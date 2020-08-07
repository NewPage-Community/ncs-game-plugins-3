void InitNative()
{
    CreateNative("NCS_Aura_Preview", Native_Preview);
}

public int Native_Preview(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    int id = GetNativeCell(2);
    SetClientAura(client, id, true);
    return 0;
}