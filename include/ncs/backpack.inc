#if defined _NCS_Backpack_
	#endinput
#endif
#define _NCS_Backpack_

enum struct Item
{
    int id;
    int amount;
    int length;
}

methodmap Items < ArrayList
{
    public Items()
    {
        Item t;
        return view_as<Items>(new ArrayList(sizeof(t), 0));
    }

    public int Add(const Item item)
    {
        this.PushArray(item, sizeof(item));
    }

    public int Get(int index, Item buffer)
    {
        return this.GetArray(index, buffer, sizeof(buffer));
    }
}

native void NCS_Backpack_AddItems(int client, Items items);

public SharedPlugin __pl_ncs_backpack = 
{
    name = "NCS-Backpack",
    file = "np-backpack.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_backpack_SetNTVOptional()
{
    MarkNativeAsOptional("NCS_Backpack_AddItems");
}
#endif