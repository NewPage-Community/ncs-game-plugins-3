#define Money_Service "user/money"

HTTPClient httpClient;

void InitAPI()
{
    httpClient = NewNCSAPI();
}

void CloseAPI()
{
    delete httpClient;
}

void GiveMoney(int client, int money, const char[] reason)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, MAX_UID_LENGTH))
        return;

    // JSON data
    JSONObject data = new JSONObject();
    data.SetInt64("uid", uid);
    data.SetInt("money", money);
    data.SetString("reason", reason);

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteCell(money, false);
    dp.WriteString(reason, false);

    char path[128];
    Format(path, sizeof(path), "%s/%s/give", Money_Service, uid);
    httpClient.Post(path, data, GiveMoneyCallback, dp);
}

public void GiveMoneyCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    char uid[MAX_UID_LENGTH], reason[256];
    dp.Reset();
    int client = dp.ReadCell();
    dp.ReadString(uid, sizeof(uid));
    int money = dp.ReadCell();
    dp.ReadString(reason, sizeof(reason));
    dp.Close();

    if (CheckAPIError("Money", "GiveMoneyCallback", response, error))
    {
        NCS_LogError("Money", "GiveMoneyCallback", "#%d UID:%s GiveMoney:%d %s", client, uid, money, reason);
        return;
    }
}