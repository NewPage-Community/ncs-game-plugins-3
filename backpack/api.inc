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