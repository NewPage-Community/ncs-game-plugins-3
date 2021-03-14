#define Backpack_Service "backpack/user"

void DisplayUserSkins(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
    {
        NCS_Chat(client, _, "您的用户数据尚未加载，请重试");
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
    if (CheckAPIError("Skin", "UserSkinsCallback", response, error))
        return;

    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[MAX_UID_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    dp.Close();

    // JSON
    char iUID[MAX_UID_LENGTH];
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray items = view_as<JSONArray>(data.Get("items"));
    int length = items.Length;

    // Check UID is valid
    data.GetString("uid", iUID, sizeof(iUID));
    if (!StrEqual(iUID, uid) || !IsValidClient(client))
    {
        delete items;
        return;
    }
    
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
        {
            delete item;
            continue;
        }
        delete item;
        
        char desc[64];
        int left = StringToInt(exprTime) - GetTime();
        if (left > 0)
            Format(desc, sizeof(desc), "%s(%d天%d小时)", s.name, left / 86400, (left % 86400) / 3600);
        else
            Format(desc, sizeof(desc), "%s", s.name);
        bool used = g_UsersUsedSkin[client] == s.id;
        if (used)
            Format(desc, sizeof(desc), "(使用中)%s", desc);
        menu.AddItem(skinID, desc, used ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);
    }
    menu.Display(client, MENU_TIME_FOREVER);

    delete items;
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

        SetUsedSkin(client, skinID);
        if (engine == Engine_CSGO)
            SetModel(client);
        NCS_Chat(client, _, "已成功更换为 {lime}%s{default}，可通过 {olive}!tp{default} 查看模型", skinName);
    }
}