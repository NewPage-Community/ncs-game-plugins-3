static PassInfo player[MAXPLAYERS+1];

void DisplayPassInfo(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;
    
    char path[128];
    Format(path, sizeof(path), "%s/info/%s", GamePass_Service, uid);
    httpClient.Get(path, LoadPassInfo, client);
}

public void LoadPassInfo(HTTPResponse response, int client, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Pass", "LoadPassInfo", "Faild to load info %d (%s)", response.Status, error);
        return;
    }
    
    if (!IsValidClient(client))
        return;

    // JSON data
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONObject info = view_as<JSONObject>(data.Get("info"));

    player[client].type = info.GetInt("type");
    player[client].point = info.GetInt("point");
    player[client].box_amount = info.GetInt("box_amount");

    int type = player[client].type;
    int point = player[client].point;
    char progress[16];
    ProgressBar(point, progress, sizeof(progress));

    Menu menu = new Menu(PassInfoMenuHandle);
    menu.SetTitle("通行证 - 第%d赛季\n通行证类型: %s\n等级: %d\n%s\n ", season, passTypeName[type], Level(point), progress);
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
}

void UsePassPointBox(int client)
{
    if (!IsValidClient(client))
        return;
    
    int level = Level(player[client].point);
    Menu menu = new Menu(UsePassPointBoxHandle);
    menu.SetTitle("使用经验箱\n等级: %d\n经验箱: %d个", level, player[client].box_amount);
    menu.AddItem("", "使用1个经验箱", player[client].box_amount > 0 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.AddItem("", "使用全部经验箱", player[client].box_amount > 0 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
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
                player[client].point += 7200;
                player[client].box_amount --;
                UsePassPointBox(client);
            }
            case 1:
            {
                UsePointBox(client, true);
                DisplayPassInfo(client);
            }
        }
    }
}

void DisplayNextRewards(int client)
{
    if (!IsValidClient(client))
        return;
    
    int level = Level(player[client].point);
    Menu menu = new Menu(NextRewardsHandle);
    menu.SetTitle("通行证下一级奖励\n%d级\n ", level);

    char desc[512];

    // free
    Format(desc, sizeof(desc), "免费版:\n");
    for (int i = 0; i < MAX_LEVEL_REWARDS; i++)
    {
        if (rewards[level][FreeReward][i].id == 0)
            break;
        
        char name[MAX_ITEM_NAME_LENGTH];
        rewards[level][FreeReward][i].GetName(name, sizeof(name));
        Format(desc, sizeof(desc), "- %s\n", name);
    }
    menu.AddItem("", desc, ITEMDRAW_DISABLED);

    // adv
    Format(desc, sizeof(desc), "高级版:\n");
    for (int i = 0; i < MAX_LEVEL_REWARDS; i++)
    {
        if (rewards[level][FreeReward][i].id == 0)
            break;
        
        char name[MAX_ITEM_NAME_LENGTH];
        rewards[level][AdvReward][i].GetName(name, sizeof(name));
        Format(desc, sizeof(desc), "- %s\n", name);
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
}