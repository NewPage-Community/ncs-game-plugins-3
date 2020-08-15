void LoadRewards()
{
    ClearRewards();
    
    char path[128];
    Format(path, sizeof(path), "%s/rewards", GamePass_Service);
    httpClient.Get(path, LoadRewardsCallback);
}

public void LoadRewardsCallback(HTTPResponse response, int client, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Pass", "LoadPassInfo", "Faild to load info %d (%s)", response.Status, error);
        return;
    }
    
    // JSON data
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray free = view_as<JSONArray>(data.Get("free_rewards"));
    JSONArray adv = view_as<JSONArray>(data.Get("adv_rewards"));

    season = data.GetInt("season");

    // free
    int length = free.Length;
    int lastLevel = 0;
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        Reward reward;
        JSONObject item = view_as<JSONObject>(free.Get(i));
        reward.id = item.GetInt("id");
        reward.level = item.GetInt("level");
        reward.amount = item.GetInt("amount");

        char ilength[INT64_LENGTH];
        item.GetString("length", ilength, sizeof(ilength));
        reward.length = StringToInt(ilength);
        item.GetString("name", reward.name, MAX_ITEM_NAME_LENGTH);

        if (lastLevel != reward.level)
        {
            lastLevel = reward.level;
            count = 0;
        }

        rewards[reward.level][FreeReward][count++] = reward;
        delete item;
    }

    // adv
    length = adv.Length;
    lastLevel = 0;
    count = 0;
    for (int i = 0; i < length; i++)
    {
        Reward reward;
        JSONObject item = view_as<JSONObject>(adv.Get(i));
        reward.id = item.GetInt("id");
        reward.level = item.GetInt("level");
        reward.amount = item.GetInt("amount");
        reward.length = item.GetInt("length");
        item.GetString("name", reward.name, MAX_ITEM_NAME_LENGTH);

        if (lastLevel != reward.level)
        {
            lastLevel = reward.level;
            count = 0;
        }
        
        rewards[reward.level][AdvReward][count++] = reward;
        delete item;
    }

    delete free;
    delete adv;
}

void ClearRewards()
{
    Reward empty;
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        for (int j = 0; j < MAX_LEVEL_REWARDS; j++)
        {
            rewards[i][FreeReward][j] = empty;
            rewards[i][AdvReward][j] = empty;
        }
    }
}