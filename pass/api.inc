#define PassUser_Service "pass/user"
#define GamePass_Service "game/pass"

HTTPClient httpClient;

static Handle updatePointTimer;

void InitAPI()
{
    httpClient = NewNCSAPI();
    updatePointTimer = CreateTimer(60.0, Timer_UpdatePoint, 0, TIMER_REPEAT);
}

void CloseAPI()
{
    delete updatePointTimer;
    delete httpClient;
}

public Action Timer_UpdatePoint(Handle timer, int client)
{
    JSONObject data = new JSONObject();
    JSONArray array = new JSONArray();

    int count = 0;
    if (client != 0)
    {
        if (player[client].IsValid() && player[client].point > 0)
        {
            player[client].PushJSONArray(array);
            player[client].point = 0;
            count++;
        }
    }
    else
    {
        int length = sizeof(player);
        for (int i = 0; i < length; i++)
        {
            if (player[i].IsValid() && player[i].point > 0)
            {
                player[i].PushJSONArray(array);
                count++;
            }
            player[i].point = 0;
        }
    }
    
    data.Set("add", array);
    delete array;

    // Do not send empty data
    if (count > 0)
    {
        char path[128];
        Format(path, sizeof(path), "%s/addpoints", PassUser_Service);
        httpClient.Post(path, data, UpdatePointCallback);
    }
    else
        delete data;
    
    return Plugin_Continue;
}

public void UpdatePointCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Pass", "UpdatePointCallback", response, error);
}

void UsePointBox(int client, bool all = false)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteCell(all, false);

    // JSON data
    JSONObject data = new JSONObject();
    data.SetInt64("uid", uid);
    data.SetBool("all", all);
    
    char path[128];
    Format(path, sizeof(path), "%s/pointbox", GamePass_Service);
    httpClient.Post(path, data, UsePointBoxCallback, dp);
}

public void UsePointBoxCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[MAX_UID_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    bool all = dp.ReadCell();
    dp.Close()

    if (CheckAPIError("Pass", "UsePointBoxCallback", response, error))
    {
        NCS_LogError("Pass", "UsePointBoxCallback", "#%d UID:%s All:%d", client, uid, all);

        if (IsValidClient(client))
            NCS_Chat(client, _, "使用通行证经验箱失败");
        return;
    }

    if (IsValidClient(client))
            NCS_Chat(client, _, "成功使用通行证经验箱");
}