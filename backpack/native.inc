void InitNative()
{
    CreateNative("NCS_Backpack_AddItems", Native_AddItems);
    CreateNative("NCS_Backpack_HaveItem", Native_HaveItem);
}

public int Native_AddItems(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    Items items = view_as<Items>(GetNativeCell(2));

    AddItems(client, items);
    return 0;
}

public int Native_HaveItem(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    int itemID = GetNativeCell(2);
    Function callback = GetNativeFunction(3);
    any data = GetNativeCell(4);
    HaveUserItem(client, itemID, plugin, callback, data);
    return 0;
}