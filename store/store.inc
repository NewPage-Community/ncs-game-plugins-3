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
        return RoundToNearest(this.price * this.discount);
    }

    void GetMenuItem(char[] info, int info_size, char[] desc, int desc_size)
    {
        IntToString(this.id, info, info_size);
        if (FloatCompare(1.0, this.discount) == 1)
            Format(desc, desc_size, "%s (%1.1f折)", this.name, this.discount * 10.0);
        else
            Format(desc, desc_size, "%s", this.name);
    }
}

methodmap Items < ArrayList
{
    public Items(JSONArray items)
    {
        Item t;
        ArrayList list = new ArrayList(sizeof(t), 0);
        int length = items.Length;
        for (int i = 0; i < length; i++)
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
            
            // Fix discount
            if (FloatCompare(0.0, item.discount) == 0)
                item.discount = 1.0;

            list.PushArray(item, sizeof(item));
        }
        return view_as<Items>(list);
    }

    public int Get(int index, Item buffer)
    {
        return this.GetArray(index, buffer, sizeof(buffer));
    }

    // not found return -1
    public int Find(int id, Item buffer)
    {
        int length = this.Length;
        for (int i = 0; i < length; i++)
        {
            Item item;
            this.Get(i, item);
            if (item.id == id)
            {
                buffer = item;
                return i;
            }
        }
        return -1;
    }
}

Items playerStoreList[MAXPLAYERS+1];

char typeName[][] = {"", "皮肤", "拖尾", "喷漆", "投掷物轨迹"};
char passTypeName[][] = {"", "高级通行证", "终极通行证"};
char passTypeDesc[][] = {"", "- 解锁高级版等级奖励", "- 解锁高级版等级奖励\n- 通行证等级增加20级"};
int passTypePrice[] = {0, 6888, 9888};

#include "store/api"
#include "store/menu"
#include "store/cmd"