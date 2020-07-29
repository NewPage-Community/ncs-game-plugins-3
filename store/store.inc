#define MAX_ITEM_NAME_LENGTH 32
#define MAX_ITEM_DESC_LENGTH 64
#define Store_Service "game/store"

enum struct Item
{
    int id;
    char name[MAX_ITEM_NAME_LENGTH];
    char description[MAX_ITEM_DESC_LENGTH];
    int type;
    int price;
    float discount;
    bool available;
    bool alreadyHave;

    int GetPrice()
    {
        return view_as<int>(this.price * this.discount);
    }
}

methodmap Items < ArrayList
{
    public Items(JSONArray items)
    {
        ArrayList list = new ArrayList(sizeof(Item), 0);
        for (int i = 0; i < items.Length; i++)
        {
            Item item;
            JSONObject data = view_as<JSONObject>(items.Get(i));
            item.id = data.GetInt("item_id");
            data.GetString("name", item.name, MAX_ITEM_NAME_LENGTH);
            data.GetString("description", item.description, MAX_ITEM_DESC_LENGTH);
            item.type = data.GetInt("type");
            item.price = data.GetInt("price");
            item.discount = data.GetFloat("discount");
            item.available = data.GetBool("available");
            item.alreadyHave = data.GetBool("already_have");
            list.PushArray(item, sizeof(item));
        }
        return view_as<Items>(list);
    }

    public int Get(int index, Item buffer)
    {
        return this.GetArray(index, buffer, sizeof(buffer));
    }

    public int Find(int id)
    {
        int length = this.Length;
        for (int i = 0; i < length; i++)
        {
            Item item;
            this.Get(i, item);
            if (item.id == id)
                return i;
        }
        return 0;
    }
}

Items playerStoreList[MAXPLAYERS+1];

char typeName[][] = {"", "皮肤", "拖尾", "喷漆", "投掷物轨迹"};

#include "store/api"
#include "store/menu"
#include "store/cmd"