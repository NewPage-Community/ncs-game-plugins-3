#define CachePath "data/ncs-stats-cache.json"
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
            static char path[128];
            Format(path, sizeof(path), "%s/user/incrs", Stats_Service);
            NewNCSAPI(path).Post(data, InitCacheCallback);
        }
    }
    
    cache = CreateTimer(1.0, Timer_Cache, 0, TIMER_REPEAT);
}

public void InitCacheCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Stats", "InitCacheCallback", response, error);
}

void CloseCache()
{
    delete cache;
}

public Action Timer_Cache(Handle timer)
{
    char json[MaxCacheLength];
    JSONObject data = StatsList.JSON();
    data.ToString(json, sizeof(json), JSON_COMPACT);
    delete data;

    // Save to file
    File file = OpenFile(cacheFile, "w+");
    file.WriteString(json, false);
    file.Flush();
    delete file;

    return Plugin_Continue;
}
