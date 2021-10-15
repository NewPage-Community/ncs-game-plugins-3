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

void GetStatsData(int client, Function callback, const char[] range, const char[] statsName, const char[] version)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, MAX_UID_LENGTH))
        return;

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteFunction(callback, false);
    dp.WriteString(statsName, false);
    dp.WriteString(version, false);
    dp.WriteString(range, false);

    char path[128];
    Format(path, sizeof(path), "%s/user/%s/%s/%s/%s", Stats_Service, uid, statsName, version, range);
    httpClient.Get(path, GetStatsDataCallback, dp);
}

public void GetStatsDataCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    StatsData stats;
    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[MAX_UID_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    Function callback = dp.ReadFunction();
    dp.ReadString(stats.statsName, NCS_STATS_KEY_LENGTH);
    dp.ReadString(stats.version, NCS_STATS_KEY_LENGTH);
    dp.ReadString(stats.range, NCS_STATS_KEY_LENGTH);
    dp.Close()

    if (CheckAPIError("Stats", "GetStatsDataCallback", response, error))
        return;

    // JSON
    char buffer[INT64_LENGTH];
    JSONObject data = view_as<JSONObject>(response.Data);
    stats.score = data.GetFloat("score");
    data.GetInt64("rank", buffer, sizeof(buffer));
    stats.rank = StringToInt(buffer);
    data.GetInt64("total", buffer, sizeof(buffer));
    stats.total = StringToInt(buffer);

    Call_StartFunction(INVALID_HANDLE, callback);
    Call_PushCell(client);
    Call_PushArray(stats, sizeof(stats));
    Call_Finish();
}
