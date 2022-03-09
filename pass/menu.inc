// Local Cached
static PassInfo playerCached[MAXPLAYERS + 1];

void DisplayPassInfo(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;
    
    char path[128];
    Format(path, sizeof(path), "%s/info/%s", GamePass_Service, uid);
    NewNCSAPI(path).Get(LoadPassInfo, client);
}

public void LoadPassInfo(HTTPResponse response, int client, const char[] error)
{
    if (CheckAPIError("Pass", "LoadPassInfo", response, error))
        return;
    
    if (!IsValidClient(client))
        return;

    // JSON data
    JSONObject data = view_as<JSONObject>(response.Data);

    playerCached[client].type = data.GetInt("type");
    playerCached[client].point = data.GetInt("point");
    playerCached[client].box_amount = data.GetInt("box_amount");

    int type = playerCached[client].type;
    int point = playerCached[client].point;
    char progress[64];
    char startDate[20], endDate[20];
    ProgressBar(point, progress, sizeof(progress));
    FormatTime(startDate, sizeof(startDate), "%Y/%m/%d", cv_pass_starttime.IntValue);
    FormatTime(endDate, sizeof(endDate), "%Y/%m/%d", cv_pass_endtime.IntValue);

    Menu menu = new Menu(PassInfoMenuHandle);
    menu.SetTitle("通行证 - 第%d赛季\n时间: %s - %s\n类型: %s\n等级: %d\n%s\n ", season, startDate, endDate, passTypeName[type], Level(point), progress);
    menu.AddItem("", "升级通行证");
    menu.AddItem("", "使用经验箱");
    menu.AddItem("", "通行证奖励预览");
    menu.Display(client, MENU_TIME_FOREVER);
}

public int PassInfoMenuHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Select)
    {
        switch (slot)
        {
            case 0: FakeClientCommandEx(client, "sm_store");
            case 1: UsePassPointBox(client);
            case 2: DisplayRewards(client);
        }
    }
    return 0;
}

void UsePassPointBox(int client)
{
    if (!IsValidClient(client))
        return;
    
    int level = Level(playerCached[client].point);
    Menu menu = new Menu(UsePassPointBoxHandle);
    menu.SetTitle("使用经验箱\n等级: %d\n经验箱: %d个", level, playerCached[client].box_amount);
    menu.AddItem("", "使用1个经验箱", playerCached[client].box_amount > 0 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.AddItem("", "使用全部经验箱", playerCached[client].box_amount > 0 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int UsePassPointBoxHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End && slot == MenuEnd_Exit)
    {
        delete menu;
    }
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        DisplayPassInfo(client);
        delete menu;
    }
    else if (action == MenuAction_Select)
    {
        switch (slot)
        {
            case 0:
            {
                UsePointBox(client, false);
                playerCached[client].point += 7200;
                playerCached[client].box_amount --;
                UsePassPointBox(client);
            }
            case 1:
            {
                UsePointBox(client, true);
                DisplayPassInfo(client);
            }
        }
    }
    return 0;
}

void DisplayRewards(int client)
{
    if (!IsValidClient(client))
        return;
    
    int level = Level(playerCached[client].point) + 1;
    Menu menu = new Menu(RewardsHandle);
    menu.SetTitle("通行证奖励预览", level);

    char desc[512];
    char name[MAX_ITEM_NAME_LENGTH];

    for (int i = 1; i < MAX_LEVEL; i++)
    {
        Format(desc, sizeof(desc), "等级 %d\n免费版: ", i);
        if (rewards[i][FreeReward].IsValid())
        {
            rewards[i][FreeReward].GetName(name, sizeof(name));
            Format(desc, sizeof(desc), "%s%s", desc, name);
        }
        else
        {
            Format(desc, sizeof(desc), "%s无", desc);
        }
        Format(desc, sizeof(desc), "%s\n高级版: ", desc);
        if (rewards[i][AdvReward].IsValid())
        {
            rewards[i][AdvReward].GetName(name, sizeof(name));
            Format(desc, sizeof(desc), "%s%s", desc, name);
        }
        else
        {
            Format(desc, sizeof(desc), "%s无", desc);
        }
        menu.AddItem("", desc, ITEMDRAW_DISABLED);
    }

    menu.Pagination = RoundToCeil(float(level) / float(6));
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int RewardsHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End && slot == MenuEnd_Exit)
    {
        delete menu;
    }
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        DisplayPassInfo(client);
        delete menu;
    }
    return 0;
}
