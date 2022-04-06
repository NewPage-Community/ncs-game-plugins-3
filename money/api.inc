#define Money_Service "user/money"

#define ErrNoEnoughMoney "no enough money"
#define ErrReachGiftLimit "reach gift limit"

void InitAPI()
{
    InitNCSAPI();
}

void CloseAPI()
{

}

void GiveMoney(int client, int money, const char[] reason)
{
    static char uid[MAX_UID_LENGTH];
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

    static char path[128];
    Format(path, sizeof(path), "%s/%s/give", Money_Service, uid);
    NewNCSAPI(path).Post(data, GiveMoneyCallback, dp);
}

public void GiveMoneyCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    static char uid[MAX_UID_LENGTH], reason[256];
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

void GiftMoney(int client, int target, int money)
{
    static char uid[MAX_UID_LENGTH], target_uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, MAX_UID_LENGTH))
        return;
    if (!NCS_Account_GetUID(target, target_uid, MAX_UID_LENGTH))
        return;

    // JSON data
    JSONObject data = new JSONObject();
    data.SetInt64("uid", uid);
    data.SetInt64("target", target_uid);
    data.SetInt("money", money);

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);
    dp.WriteString(target_uid, false);
    dp.WriteCell(money, false);

    static char path[128];
    Format(path, sizeof(path), "%s/%s/gift", Money_Service, uid);
    NewNCSAPI(path).Post(data, GiftMoneyCallback, dp);
}

public void GiftMoneyCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    // DataPack
    static char uid[MAX_UID_LENGTH], target_uid[MAX_UID_LENGTH];
    dp.Reset();
    int client = dp.ReadCell();
    dp.ReadString(uid, sizeof(uid));
    dp.ReadString(target_uid, sizeof(target_uid));
    int money = dp.ReadCell();
    dp.Close();

    if (CheckAPIError("Money", "GiftMoneyCallback", response, error))
    {
        if (IsValidClient(client))
        {
            if (IsError(response.Data, ErrNoEnoughMoney, 15))
                NCS_Chat(client, _, "您的软妹币余额不足");
            else if (IsError(response.Data, ErrReachGiftLimit, 16))
            {
                static char err[32];
                JSONObject json = view_as<JSONObject>(response.Data);
                json.GetString("error", err, sizeof(err));
                NCS_Chat(client, _, "超出今日赠送软妹币上限 {green}200/%s", err[17]);
            }
            else
                NCS_Chat(client, _, "赠送失败");
        }
        NCS_LogError("Money", "GiftMoneyCallback", "#%d UID:%s Target:%d GiftMoney: %d", client, uid, target_uid, money);
        return;
    }

    if (IsValidClient(client))
    {
        JSONObject json = view_as<JSONObject>(response.Data);
        int remaining = json.GetInt("remaining");
        int target = NCS_Account_GetClientOfUID(target_uid);
        if (target)
            NCS_Chat(client, _, "成功赠送 %N %d 软妹币, 今天剩余额度: %d 软妹币", target, money, remaining);
        else
            NCS_Chat(client, _, "成功赠送 UID:%d %d 软妹币, 今天剩余额度: %d 软妹币", target_uid, money, remaining);
    }
}

bool IsError(JSON data, const char[] err, int size)
{
    if (data == null)
        return false;

    static char error[512];
    JSONObject json = view_as<JSONObject>(data);
    json.GetString("error", error, sizeof(error));
    return !strncmp(error, err, size);
}