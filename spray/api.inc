#define BackpackItems_Service "backpack/items"
#define BackpackUser_Service "backpack/user"

HTTPClient httpClient;

void InitAPI()
{
    httpClient = NewNCSAPI();
}

void CloseAPI()
{
    delete httpClient;
}

void LoadSpray()
{
    char path[128];
    Format(path, sizeof(path), "%s/type/4", BackpackItems_Service);
    httpClient.Get(path, LoadSprayInfo);
}

public void LoadSprayInfo(HTTPResponse response, DataPack dp, const char[] error)
{
    if (CheckAPIError("Spray", "LoadSprayInfo", response, error))
        return;

    // JSON
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray items = view_as<JSONArray>(data.Get("items"));
    int length = items.Length;

    // Loading sprays
    if (sprays != INVALID_HANDLE)
        delete sprays;
    sprays = new SprayIndex();

    for (int i = 0; i < length; i++)
    {
        JSONObject item = view_as<JSONObject>(items.Get(i));
        sprays.Add(item);
        delete item;
    }
    
    delete items;
}

void CheckUserHaveSpray(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    int id = g_UsersUsedSpray[client];
    
    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteCell(id, false);
    
    char path[128];
    Format(path, sizeof(path), "%s/have/%s/%d", BackpackUser_Service, uid, id);
    httpClient.Get(path, CheckUserHaveItemCallback, dp);
}

public void CheckUserHaveItemCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    if (CheckAPIError("Spray", "CheckUserHaveItemCallback", response, error))
        return;

    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    int id = dp.ReadCell();
    dp.Close();

    if (!IsValidClient(client))
        return;
    
     // JSON
    JSONObject data = view_as<JSONObject>(response.Data);
    bool have = data.GetBool("have");

    if (g_UsersUsedSpray[client] == id && !have)
        g_UsersUsedSpray[client] = 0;
}