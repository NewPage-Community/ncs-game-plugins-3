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

void GetStatsData(int client, Handle plugin, Function callback, const char[] range, const char[] statsName, const char[] version, any data)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, MAX_UID_LENGTH))
        return;

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client);
    dp.WriteString(uid);
    dp.WriteCell(plugin);
    dp.WriteFunction(callback);
    dp.WriteCell(data);

    char path[128];
    Format(path, sizeof(path), "%s/user/%s/%s/%s/%s", Stats_Service, uid, statsName, version, range);
    httpClient.Get(path, GetStatsDataCallback, dp);
}

public void GetStatsDataCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[MAX_UID_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    Handle plugin = dp.ReadCell();
    Function callback = dp.ReadFunction();
    any cdata = dp.ReadCell();
    dp.Close()

    if (CheckAPIError("Stats", "GetStatsDataCallback", response, error))
        return;

    // JSON
    JSONObject data = view_as<JSONObject>(response.Data);
    float score = data.GetFloatEx("score");
    int rank = data.GetInt64Int("rank");
    int total = data.GetInt64Int("total");

    Call_StartFunction(plugin, callback);
    Call_PushCell(client);
    Call_PushCell(score);
    Call_PushCell(rank);
    Call_PushCell(total);
    Call_PushCell(cdata);
    Call_Finish();
}
