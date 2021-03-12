HTTPClient httpClient;

static Handle updateStats;

void InitAPI()
{
    httpClient = NewNCSAPI();
    StatsList = new Stats();
    updateStats = CreateTimer(60.0, Timer_UpdateStats, 0, TIMER_REPEAT);
}

void CloseAPI()
{
    delete updateStats;

    UpdateStatsData();
    
    delete httpClient;
    delete StatsList;
}

void UpdateStatsData()
{
    // Do not send empty data
    if (StatsList.Length == 0)
        return;

    JSONObject data = StatsList.JSON();
    StatsList.Clear();

    char path[128];
    Format(path, sizeof(path), "%s/user/incrs", Stats_Service);
    httpClient.Post(path, data, UpdateStatsDataCallback);
}

public void UpdateStatsDataCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Stats", "UpdateStatsDataCallback", response, error);
}

public Action Timer_UpdateStats(Handle timer)
{
    UpdateStatsData();
    return Plugin_Continue;
}
