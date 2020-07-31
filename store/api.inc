HTTPClient httpClient;

void InitAPI()
{
    httpClient = NewNCSAPI();
}

void CloseAPI()
{
    delete httpClient;
}

void LoadUserStoreList(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    char path[128];
    Format(path, sizeof(path), "%s/list/%s", Store_Service, uid);
    httpClient.Get(path, LoadUserStoreListCallback, client);
}

public void LoadUserStoreListCallback(HTTPResponse response, int client, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Store", "LoadUserStoreListCallback", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

    if (!IsValidClient(client))
        return;

    // JSON data
    JSONObject data = view_as<JSONObject>(response.Data);
    if (playerStoreList[client] != INVALID_HANDLE)
        delete playerStoreList[client];
    
    JSONArray array = view_as<JSONArray>(data.Get("items"));
    playerStoreList[client] = new Items(array);
    delete array;

    playerMoney[client] = data.GetInt("money");
    playerPass[client] = data.GetInt("pass_type");
}

void BuyItem(int client, Item item)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteString(item.name, false);

    // JSON
    JSONObject data = new JSONObject();
    data.SetInt64("uid", uid);
    data.SetInt("item_id", item.id);
    data.SetInt("price", item.GetPrice());
    
    char path[128];
    Format(path, sizeof(path), "%s/buy", Store_Service);
    httpClient.Post(path, data, BuyItemCallback, dp);
}

public void BuyItemCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[INT64_LENGTH], itemName[MAX_ITEM_NAME_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    dp.ReadString(itemName, sizeof(itemName));
    dp.Close()

    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Store", "BuyItemCallback", "Faild to load info %d (%s)", response.Status, error);
        NCS_LogError("Store", "BuyItemCallback", "#%d UID:%s", client, uid);
        if (IsValidClient(client))
            NCS_Chat(client, _, "{blue}购买失败！");
        return;
    }

    if (IsValidClient(client))
    {
        LoadUserStoreList(client);
        NCS_Chat(client, _, "{blue}购买 {green}%s{blue} 成功！", itemName);
    }
}

void BuyAdvPass(int client, int type)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteString(passTypeName[type], false);

    // JSON
    JSONObject data = new JSONObject();
    data.SetInt64("uid", uid);
    data.SetInt("type", type);
    
    char path[128];
    Format(path, sizeof(path), "%s/buy/pass", Store_Service);
    httpClient.Post(path, data, BuyItemCallback, dp);
}