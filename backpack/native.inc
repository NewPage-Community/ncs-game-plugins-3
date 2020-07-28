void InitNative()
{
    CreateNative("NCS_Backpack_AddItems", Native_AddItems);
}

public int Native_AddItems(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    Items items = view_as<Items>(GetNativeCell(2));

    AddItems(client, items);
}