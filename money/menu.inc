void DisplayMoneyRecord(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;
    
    char path[128];
    Format(path, sizeof(path), "%s/%s/records", Money_Service, uid);
    httpClient.Get(path, LoadMoneyRecord, client);
}

public void LoadMoneyRecord(HTTPResponse response, int client, const char[] error)
{
    if (CheckAPIError("Money", "LoadMoneyRecord", response, error))
        return;

    MenuEx menu = new MenuEx(MoneyRecordHandle);
    menu.SetTitle("软妹币流水帐");

    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray records = view_as<JSONArray>(data.Get("records"));

    int length = records.Length;
    for (int i; i < length; i++)
    {
        char timestamp[INT64_LENGTH], reason[256], sTime[256];
        JSONObject record = view_as<JSONObject>(records.Get(i));
        record.GetString("timestamp", timestamp, sizeof(timestamp));
        int amount = record.GetInt("amount");
        record.GetString("reason", reason, sizeof(reason));
        delete record;

        FormatTime(sTime, sizeof(sTime), NULL_STRING, StringToInt(timestamp));
        menu.AddItemEx(ITEMDRAW_DISABLED, "", "金额: %d\n时间: %s\n原因: %s", amount, sTime, reason);
    }

    delete records;

    menu.Display(client, 0);
}

public int MoneyRecordHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
}