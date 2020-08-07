#define BackpackItems_Service "backpack/items"

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