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

#include "backpack/api"
#include "backpack/native"