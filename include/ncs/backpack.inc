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
        return view_as<Items>(new ArrayList(0, 1));
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
    required = 1,
};