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

void LoadTrail()
{
    char path[128];
    Format(path, sizeof(path), "%s/type/2", BackpackItems_Service);
    httpClient.Get(path, LoadTrailInfo);
}

public void LoadTrailInfo(HTTPResponse response, DataPack dp, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Trail", "LoadTrailInfo", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

    // JSON
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray items = view_as<JSONArray>(data.Get("items"));
    int length = items.Length;

    // Loading skins
    if (trails != INVALID_HANDLE)
        delete trails;
    trails = new TrailIndex();

    for (int i = 0; i < length; i++)
    {
        JSONObject item = view_as<JSONObject>(items.Get(i));
        trails.Add(item);
        delete item;
    }
    
    delete items;
}

void CheckUserHaveTrail(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    int id = g_UsersUsedTrail[client];
    
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
        NCS_LogError("Trail", "CheckUserHaveItemCallback", "Faild to load info %d (%s)", response.Status, error);
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

    if (g_UsersUsedTrail[client] == id && !have)
        g_UsersUsedTrail[client] = 0;
}