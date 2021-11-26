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
    menu.AddItem("", "升级通行证", type == 0 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.AddItem("", "使用经验箱");
    menu.AddItem("", "查看下一级奖励");
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
            case 2: DisplayNextRewards(client);
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

void DisplayNextRewards(int client)
{
    if (!IsValidClient(client))
        return;
    
    int level = Level(playerCached[client].point) + 1;
    Menu menu = new Menu(NextRewardsHandle);
    menu.SetTitle("通行证下一级奖励\n \n%d级奖励\n ", level);

    char desc[512];

    // free
    Format(desc, sizeof(desc), "免费版:\n");
    for (int i = 0; i < MAX_LEVEL_REWARDS && level < MAX_LEVEL; i++)
    {
        if (rewards[level][FreeReward][i].name[0] == '\0')
            break;
        
        char name[MAX_ITEM_NAME_LENGTH];
        rewards[level][FreeReward][i].GetName(name, sizeof(name));
        Format(desc, sizeof(desc), "%s- %s\n", desc, name);
    }
    Format(desc, sizeof(desc), "%s\n ", desc);
    menu.AddItem("", desc, ITEMDRAW_DISABLED);

    // adv
    Format(desc, sizeof(desc), "高级版:\n");
    for (int i = 0; i < MAX_LEVEL_REWARDS && level < MAX_LEVEL; i++)
    {
        if (rewards[level][FreeReward][i].name[0] == '\0')
            break;
        
        char name[MAX_ITEM_NAME_LENGTH];
        rewards[level][AdvReward][i].GetName(name, sizeof(name));
        Format(desc, sizeof(desc), "%s- %s\n", desc, name);
    }
    menu.AddItem("", desc, ITEMDRAW_DISABLED);

    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int NextRewardsHandle(Menu menu, MenuAction action, int client, int slot)
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