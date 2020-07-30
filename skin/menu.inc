#define Backpack_Service "backpack/user"

void DisplayUserSkins(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
    {
        NCS_Chat(client, _, "{blue}您的用户数据尚未加载，请重试！");
        return;
    }

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);

    char path[128];
    Format(path, sizeof(path), "%s/%s", Backpack_Service, uid);
    httpClient.Get(path, UserSkinsCallback, dp);
}

public void UserSkinsCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Skin", "UserSkinsCallback", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[INT64_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    dp.Close();

    // JSON
    char iUID[INT64_LENGTH];
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray items = view_as<JSONArray>(data.Get("items"));
    int length = items.Length;

    // Check UID is valid
    data.GetString("uid", iUID, sizeof(iUID));
    if (!StrEqual(iUID, uid))
        return;
    
    // Menu

    Menu menu = new Menu(Menu_SkinSelected);
    menu.ExitButton = true;
    menu.SetTitle("选择你要使用的皮肤");
    menu.AddItem("0", "默认皮肤");

    for (int i = 0; i < length; i++)
    {
        JSONObject item = view_as<JSONObject>(items.Get(i));
        char skinID[8];
        char exprTime[INT64_LENGTH];
        IntToString(item.GetInt("id"), skinID, sizeof(skinID));
        item.GetString("expr_time", exprTime, sizeof(exprTime));

        Skin s;
        if (!g_Skins.Get(item.GetInt("id"), s))
            continue;
        
        menu.AddItem(skinID, s.name);
        
    }
    menu.Display(client, 60);
}

public int Menu_SkinSelected(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
    {
        delete menu;
    }
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        delete menu;
    }
    else if (action == MenuAction_Select)
    {
        if (!IsValidClient(client))
            return;

        char skinID[32], skinName[32];
        menu.GetItem(slot, skinID, sizeof(skinID), _, skinName, sizeof(skinName));

        SetUsedSkin(client, StringToInt(skinID, 10));
        NCS_Chat(client, _, "{blue}已成功更换为 {lime}%s{blue}！可通过 {olive}!tp{blue} 查看模型", skinName);
    }
}