#define NoEnoughMoneyErr "no enough money"

HTTPClient httpClient;

void InitAPI()
{
    httpClient = NewNCSAPI();
}

void CloseAPI()
{
    delete httpClient;
}

void LoadUserStoreList(int client, bool openMenu = false)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;
    
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteCell(openMenu, false);

    char path[128];
    Format(path, sizeof(path), "%s/list/%s", Store_Service, uid);
    httpClient.Get(path, LoadUserStoreListCallback, dp);
}

public void LoadUserStoreListCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    if (CheckAPIError("Store", "LoadUserStoreListCallback", response, error))
        return;

    dp.Reset();
    int client = dp.ReadCell();
    bool openMenu = dp.ReadCell();
    dp.Close();

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

    if (openMenu)
        DisplayStoreMenu(client);
}

void BuyItem(int client, Item item)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    if (playerMoney[client] < item.GetPrice())
    {
        if (IsValidClient(client))
            NCS_Chat(client, _, "{default} 您的软妹币余额不足");
        return;
    }

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteString(item.name, false);
    dp.WriteCell(item.id, false);
    dp.WriteCell(item.GetPrice(), false);

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
    char uid[MAX_UID_LENGTH], itemName[MAX_ITEM_NAME_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    dp.ReadString(itemName, sizeof(itemName));
    int item_id = dp.ReadCell();
    int item_price = dp.ReadCell();
    dp.Close()

    if (CheckAPIError("Store", "BuyItemCallback", response, error))
    {
        if (IsValidClient(client))
        {
            if (NoEnoughMoney(response.Data))
                NCS_Chat(client, _, "{default}您的软妹币余额不足");
            else
                NCS_Chat(client, _, "{default}购买失败");
        }
        NCS_LogError("Store", "BuyItemCallback", "#%d UID:%s ItemID:%d ItemPrice:%d", client, uid, item_id, item_price);
        return;
    }

    if (IsValidClient(client))
    {
        LoadUserStoreList(client);
        NCS_Chat(client, _, "{default}购买 {green}%s{default} 成功", itemName);
    }
}

void BuyAdvPass(int client, int type)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    if (!type)
        return;

    if (playerMoney[client] < passTypePrice[type])
    {
        if (IsValidClient(client))
            NCS_Chat(client, _, "{default} 您的软妹币余额不足");
        return;
    }

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteCell(type, false);

    // JSON
    JSONObject data = new JSONObject();
    data.SetInt64("uid", uid);
    data.SetInt("type", type);
    
    char path[128];
    Format(path, sizeof(path), "%s/buy/pass", Store_Service);
    httpClient.Post(path, data, BuyPassCallback, dp);
}

public void BuyPassCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[MAX_UID_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    int type = dp.ReadCell();
    dp.Close()

    if (CheckAPIError("Store", "BuyPassCallback", response, error))
    {
        if (IsValidClient(client))
        {
            if (NoEnoughMoney(response.Data))
                NCS_Chat(client, _, "{default}您的软妹币余额不足");
            else
                NCS_Chat(client, _, "{default}升级失败");
        }
        NCS_LogError("Store", "BuyPassCallback", "#%d UID:%s Type:%d", client, uid, type);
        return;
    }

    if (IsValidClient(client))
    {
        LoadUserStoreList(client);
        NCS_Chat(client, _, "{default}升级 {green}%s{default} 成功", passTypeName[type]);
    }
}

void BuyVIP(int client, int month)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    if (playerMoney[client] < VIPPrice(month))
    {
        if (IsValidClient(client))
            NCS_Chat(client, _, "{default}您的软妹币余额不足");
        return;
    }

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteCell(month, false);

    // JSON
    JSONObject data = new JSONObject();
    data.SetInt64("uid", uid);
    data.SetInt("month", month);
    
    char path[128];
    Format(path, sizeof(path), "%s/buy/vip", Store_Service);
    httpClient.Post(path, data, BuyVIPCallback, dp);
}

public void BuyVIPCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[MAX_UID_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    int month = dp.ReadCell();
    dp.Close()

    if (CheckAPIError("Store", "BuyVIPCallback", response, error))
    {
        if (IsValidClient(client))
        {
            if (NoEnoughMoney(response.Data))
                NCS_Chat(client, _, "{default}您的软妹币余额不足");
            else
                NCS_Chat(client, _, "{default}购买错误");
        }
        NCS_LogError("Store", "BuyVIPCallback", "#%d UID:%s Month:%d", client, uid, month);
        return;
    }

    if (IsValidClient(client))
    {
        if (LibraryExists("NCS-VIP"))
            NCS_VIP_ReloadClient(client);
        NCS_Chat(client, _, "{default}续费 {green}%d个月VIP{default} 成功", month);
    }
}

bool NoEnoughMoney(JSON data)
{
    if (data == null)
        return false;

    char error[512];
    JSONObject json = view_as<JSONObject>(data);
    json.GetString("error", error, sizeof(error));
    return StrEqual(error, NoEnoughMoneyErr);
}