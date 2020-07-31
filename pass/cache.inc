#define CachePath "data/ncs-pass-cache.json"

static char cacheFile[PLATFORM_MAX_PATH];
static Handle cache;

void InitCache()
{
    BuildPath(Path_SM, cacheFile, PLATFORM_MAX_PATH, CachePath);

    if (FileExists(cacheFile))
    {
        char path[128];
        JSONObject data = JSONObject.FromFile(cacheFile);
        Format(path, sizeof(path), "%s/addpoints", PassUser_Service);
        httpClient.Post(path, data, BlankCallback);
    }
    
    cache = CreateTimer(1.0, Timer_Cache, 0, TIMER_REPEAT);
}

void CloseCache()
{
    delete cache;
    DeleteFile(cacheFile);
}

public Action Timer_Cache(Handle timer)
{
    JSONObject data = new JSONObject();
    JSONArray array = new JSONArray();

    int length = sizeof(player);
    for (int i = 0; i < length; i++)
        if (player[i].IsValid() && player[i].point > 0)
            player[i].PushJSONArray(array);

    data.Set("add", array);
    data.ToFile(cacheFile);
    delete array;
    delete data;
    return Plugin_Continue;
}
