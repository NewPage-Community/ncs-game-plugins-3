#define Backpack_Service "backpack/user"

void InitAPI()
{
    InitNCSAPI();
}

void CloseAPI()
{

}

void AddItems(int client, Items items)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    JSONObject data = new JSONObject();
    JSONArray array = new JSONArray();

    int length = items.Length;
    for (int i = 0; i < length; i++)
    {
        Item tmp;
        items.Get(i, tmp);
        JSONObject item = new JSONObject();
        item.SetInt("id", tmp.id);
        item.SetInt("amount", tmp.amount);
        item.SetInt("length", tmp.length);
        array.Push(item);
        delete item;
    }

    data.SetInt64("uid", uid);
    data.Set("items", array);
    delete array;

    char path[128];
    Format(path, sizeof(path), "%s/additem", Backpack_Service);
    NewNCSAPI(path).Post(data, AddItemsCallback);
}

public void AddItemsCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Backpack", "AddItemsCallback", response, error);
}

void HaveUserItem(int client, int itemID, Handle plugin, Function callback, any data)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client);
    dp.WriteCell(plugin);
    dp.WriteFunction(callback);
    dp.WriteCell(data);

    char path[128];
    Format(path, sizeof(path), "%s/have/%s/%d", Backpack_Service, uid, itemID);
    NewNCSAPI(path).Get(HaveUserItemCallback, dp);
}

public void HaveUserItemCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    Handle plugin = dp.ReadCell();
    Function callback = dp.ReadFunction();
    any cdata = dp.ReadCell();
    dp.Close();

    if (CheckAPIError("Backpack", "HaveUserItemCallback", response, error))
        return;

    if (!IsValidClient(client))
        return;

    // JSON
    static char buffer[20];
    JSONObject data = view_as<JSONObject>(response.Data);
    bool have = data.GetBool("have");
    JSONObject item = view_as<JSONObject>(data.Get("item"));
    int itemID = item.GetInt("id");
    int amount = item.GetInt("amount");
    item.GetString("length", buffer , sizeof(buffer));
    int length = StringToInt(buffer);
    delete item;

    Call_StartFunction(plugin, callback);
    Call_PushCell(client);
    Call_PushCell(itemID);
    Call_PushCell(have);
    Call_PushCell(amount);
    Call_PushCell(length);
    Call_PushCell(cdata);
    Call_Finish();
}