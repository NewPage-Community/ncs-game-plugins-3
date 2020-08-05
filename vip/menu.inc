void DisplayVIPMenu(int client)
{
    if (!g_VIPs[client].IsValid())
        return;

    char name[MAX_NAME_LENGTH], time[16];
    ChineseTime(time, sizeof(time));
    NCS_Account_GetName(client, name, sizeof(name));

    bool vip = g_VIPs[client].IsVIP();
    
    Menu menu = new Menu(VIPMenuHandle);
    menu.SetTitle("VIP菜单\n尊贵的%s,%s好", name, time);
    menu.AddItem("", "VIP信息");
    menu.AddItem("", "续费会员");
    menu.AddItem("", "VIP权限", vip ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.Display(client, MENU_TIME_FOREVER);
}

public int VIPMenuHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Select)
    {
        switch (slot)
        {
            case 0: VIPInfoMenu(client);
            case 1: FakeClientCommandEx(client, "sm_store");
            case 2: VIPFeatureMenu(client);
        }
    }
}

void VIPInfoMenu(int client)
{
    if (!g_VIPs[client].IsValid())
        return;
    
    bool vip = g_VIPs[client].IsVIP();
    int point = g_VIPs[client].point;
    char expireTime[128];
    FormatTime(expireTime, sizeof(expireTime), "%Y-%m-%d", StringToInt(g_VIPs[client].expireDate));
    
    PanelEx panel = new PanelEx();
    panel.SetTitle("VIP信息");
    panel.DrawTextEx(" ");

    if (vip)
        panel.DrawTextEx("到期时间: %s", expireTime);
    else
        panel.DrawTextEx("到期时间: %s", point > 0 ? "已过期" : "未开通");
    
    panel.DrawTextEx("VIP等级: %d 级", g_VIPs[client].Level());
    panel.DrawTextEx("成长值: %d 点", point);
    panel.DrawTextEx(" ");
    panel.DrawItem("返回");
    panel.DrawItem("退出");
    panel.Send(client, VIPInfoMenuHandle, MENU_TIME_FOREVER);
}

public int VIPInfoMenuHandle(Handle menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Select)
    {
        switch (slot)
        {
            case 1: DisplayVIPMenu(client);
        }
    }
}

void VIPFeatureMenu(int client)
{
    if (!g_VIPs[client].IsValid())
        return;
    
    bool vip = g_VIPs[client].IsVIP();
    int level = g_VIPs[client].Level();

    if (!vip)
    {
        NCS_Chat(client, _, "{blue}您没有权限使用该菜单");
        return;
    }

    Menu menu = new Menu(VIPFeatureMenuHandle);
    menu.SetTitle("VIP权限");
    menu.AddItem("", "名字颜色（40级）", level >= 40 && LibraryExists("NCS-Chat") ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.AddItem("", "投票踢人（60级）", level >= 60 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.AddItem("", "自定义头衔（100级）", level >= 100 && LibraryExists("NCS-Chat") ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.ExitButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int VIPFeatureMenuHandle(Handle menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End && slot == MenuEnd_Exit)
        delete menu;
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        DisplayVIPMenu(client);
        delete menu;
    }
    else if (action == MenuAction_Select)
    {
        switch (slot)
        {
            case 0: DisplayNameColorMenu(client);
            case 1: DisplayKickTargetMenu(client);
            case 2: FakeClientCommandEx(client, "sm_title");
        }
    }
}