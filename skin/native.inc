void InitNative()
{
    CreateNative("NCS_Skin_Preview", Native_Preview);
}

public int Native_Preview(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    int skinID = GetNativeCell(2);
    PreviewSkin(client, skinID);
    return 0;
}