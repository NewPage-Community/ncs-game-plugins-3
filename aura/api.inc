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

void LoadAura()
{
    char path[128];
    Format(path, sizeof(path), "%s/type/3", BackpackItems_Service);
    httpClient.Get(path, LoadAuraInfo);
}

public void LoadAuraInfo(HTTPResponse response, DataPack dp, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Aura", "LoadAuraInfo", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

    // JSON
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray items = view_as<JSONArray>(data.Get("items"));
    int length = items.Length;

    // Loading auras
    if (auras != INVALID_HANDLE)
        delete auras;
    auras = new AuraIndex();

    for (int i = 0; i < length; i++)
    {
        JSONObject item = view_as<JSONObject>(items.Get(i));
        auras.Add(item);
        delete item;
    }
    
    delete items;
}

void CheckUserHaveAura(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    int id = g_UsersUsedAura[client];
    
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
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Aura", "CheckUserHaveItemCallback", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

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

    if (g_UsersUsedAura[client] == id && !have)
        g_UsersUsedAura[client] = 0;
}