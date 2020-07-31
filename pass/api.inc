#define PassUser_Service "pass/user"

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

    if (client != 0)
    {
        player[client].PushJSONArray(array);
        player[client].point = 0;
    }
    else
    {
        int length = sizeof(player);
        for (int i = 0; i < length; i++)
        {
            if (player[i].IsValid() && player[i].point > 0)
                player[i].PushJSONArray(array);
            player[i].point = 0;
        }
    }
    
    data.Set("add", array);
    delete array;

    char path[128];
    Format(path, sizeof(path), "%s/addpoints", PassUser_Service);
    httpClient.Post(path, data, BlankCallback);
    return Plugin_Continue;
}