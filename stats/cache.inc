#define CachePath "data/ncs-stats-cache.json"

static char cacheFile[PLATFORM_MAX_PATH];
static Handle cache;

void InitCache()
{
    BuildPath(Path_SM, cacheFile, PLATFORM_MAX_PATH, CachePath);

    if (FileExists(cacheFile))
    {
        char path[128];
        JSONObject data = JSONObject.FromFile(cacheFile);
        Format(path, sizeof(path), "%s/user/incrs", Stats_Service);
        httpClient.Post(path, data, BlankCallback);
    }
    
    cache = CreateTimer(1.0, Timer_Cache, 0, TIMER_REPEAT);
}

void CloseCache()
{
    DeleteFile(cacheFile);
    delete cache;
}

public Action Timer_Cache(Handle timer)
{
    JSONObject data = StatsList.JSON();
    data.ToFile(cacheFile);
    return Plugin_Continue;
}
