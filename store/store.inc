#define MAX_ITEM_NAME_LENGTH 32
#define MAX_ITEM_DESC_LENGTH 64
#define NEWLINE "#"
#define Store_Service "game/store"
#define PASSBOX_ID 1000

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

methodmap Items < StringMap
{
	public Items(JSONArray items)
	{
		StringMap list = new StringMap();
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
			item.discount = data.GetFloatEx("discount");
			item.available = data.GetBool("available");
			item.alreadyHave = item.id == PASSBOX_ID ? false : data.GetBool("already_have");
			
			// New line
			ReplaceString(item.description, MAX_ITEM_DESC_LENGTH, NEWLINE, "\n");

			char id[8];
			IntToString(item.id, id, sizeof(id));
			list.SetArray(id, item, sizeof(item), true);
			delete data;
		}
		return view_as<Items>(list);
	}

	public bool Get(int id, Item buffer)
	{
		char cid[8];
		IntToString(id, cid, sizeof(cid));
		return this.GetArray(cid, buffer, sizeof(buffer));
	}

	public bool CGet(const char[] id, Item buffer)
	{
		return this.GetArray(id, buffer, sizeof(buffer));
	}
}

Items playerStoreList[MAXPLAYERS+1];
int playerMoney[MAXPLAYERS+1];
int playerPass[MAXPLAYERS+1];

char vipDesc[] = "详细VIP特权请查看论坛\n每续费1月送50成长值(叠加)";
char typeName[][] = {"", "皮肤", "拖尾", "光环", "喷漆"};
char passTypeName[][] = {"", "高级通行证", "终极通行证"};
char passTypeDesc[][] = {"", "- 解锁高级版等级奖励\n（2个限定皮肤、2个限定装饰）", "- 解锁高级版等级奖励\n（2个限定皮肤、2个限定装饰）\n- 通行证等级增加10级"};
int passTypePrice[] = {0, 6888, 9888};

#include "store/api"
#include "store/menu"
#include "store/cmd"
#include "store/vip"