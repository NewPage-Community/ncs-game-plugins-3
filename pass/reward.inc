void LoadRewards()
{
    ClearRewards();
    
    static char path[128];
    Format(path, sizeof(path), "%s/rewards", GamePass_Service);
    NewNCSAPI(path).Get(LoadRewardsCallback);
}

public void LoadRewardsCallback(HTTPResponse response, int client, const char[] error)
{
    if (CheckAPIError("Pass", "LoadRewardsCallback", response, error))
        return;
    
    // JSON data
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray free = view_as<JSONArray>(data.Get("free_rewards"));
    JSONArray adv = view_as<JSONArray>(data.Get("adv_rewards"));

    season = data.GetInt("season");

    // free
    int length = free.Length;
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

        rewards[reward.level][FreeReward] = reward;
        delete item;
    }

    // adv
    length = adv.Length;
    for (int i = 0; i < length; i++)
    {
        Reward reward;
        JSONObject item = view_as<JSONObject>(adv.Get(i));
        reward.id = item.GetInt("id");
        reward.level = item.GetInt("level");
        reward.amount = item.GetInt("amount");
        reward.length = item.GetInt("length");
        item.GetString("name", reward.name, MAX_ITEM_NAME_LENGTH);
        
        rewards[reward.level][AdvReward] = reward;
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
        rewards[i][FreeReward] = empty;
        rewards[i][AdvReward] = empty;
    }
}