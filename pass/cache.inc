#define CachePath "data/ncs-pass-cache.json"
#define MaxCacheLength 10240

static char cacheFile[PLATFORM_MAX_PATH];
static Handle cache;
static bool ready = false;

void InitCache()
{
    if (ready) return;
    ready = true;

    BuildPath(Path_SM, cacheFile, PLATFORM_MAX_PATH, CachePath);

    if (FileExists(cacheFile))
    {
        char json[MaxCacheLength];
        File file = OpenFile(cacheFile, "r");
        file.ReadString(json, sizeof(json));
        delete file;

        if (json[0] == '{')
        {
            JSONObject data = JSONObject.FromString(json);
            char path[128];
            Format(path, sizeof(path), "%s/addpoints", PassUser_Service);
            NewNCSAPI(path).Post(data, InitCacheCallback);
        }
    }
    
    cache = CreateTimer(1.0, Timer_Cache, 0, TIMER_REPEAT);
}

public void InitCacheCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Pass", "InitCacheCallback", response, error);
}

void CloseCache()
{
    delete cache;
}

public Action Timer_Cache(Handle timer)
{
    char json[MaxCacheLength];
    JSONObject data = new JSONObject();
    JSONArray array = new JSONArray();

    int length = sizeof(player);
    for (int i = 0; i < length; i++)
        if (player[i].IsValid() && player[i].point > 0)
            player[i].PushJSONArray(array);

    data.Set("add", array);
    data.ToString(json, sizeof(json), JSON_COMPACT);
    delete array;
    delete data;

    // Save to file
    File file = OpenFile(cacheFile, "w+");
    file.WriteString(json, false);
    file.Flush();
    delete file;

    return Plugin_Continue;
}
