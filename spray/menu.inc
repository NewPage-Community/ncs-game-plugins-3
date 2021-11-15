void DisplayUserSprays(int client)
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
    Format(path, sizeof(path), "%s/%s", BackpackUser_Service, uid);
    NewNCSAPI(path).Get(UserSpraysCallback, dp);
}

public void UserSpraysCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    if (CheckAPIError("Spray", "UserSpraysCallback", response, error))
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

    Menu menu = new Menu(Menu_Selected);
    menu.ExitButton = true;
    menu.SetTitle("选择你要使用的喷漆");
    menu.AddItem("0", "不使用");

    for (int i = 0; i < length; i++)
    {
        JSONObject item = view_as<JSONObject>(items.Get(i));
        char id[8];
        char exprTime[INT64_LENGTH];
        IntToString(item.GetInt("id"), id, sizeof(id));
        item.GetString("expr_time", exprTime, sizeof(exprTime));

        Spray s;
        if (!sprays.Get(item.GetInt("id"), s))
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
        bool used = g_UsersUsedSpray[client] == s.id;
        if (used)
            Format(desc, sizeof(desc), "(使用中)%s", desc);
        menu.AddItem(id, desc, used ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);
    }
    menu.Display(client, MENU_TIME_FOREVER);

    delete items;
}

public int Menu_Selected(Menu menu, MenuAction action, int client, int slot)
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
        if (IsValidClient(client))
        {
            char id[8], name[32];
            menu.GetItem(slot, id, sizeof(id), _, name, sizeof(name));

            SetUsedSpray(client, id);
            NCS_Chat(client, _, "已成功更换为 {lime}%s", name);
        }
    }
    return 0;
}
