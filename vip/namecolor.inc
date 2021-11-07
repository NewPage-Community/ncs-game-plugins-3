#define COOKIE_COLOR_NAME_KEY "name_color"

void DisplayNameColorMenu(int client)
{
    if (!g_VIPs[client].IsValid())
        return;
    
    Menu menu = new Menu(NameColorMenuHandle);
    menu.SetTitle("名字颜色");
    menu.AddItem("", "默认");
    menu.AddItem("{white}", "白色");
    menu.AddItem("{green}", "绿色");
    menu.AddItem("{yellow}", "黄色");
    menu.AddItem("{gray}", "灰色");
    menu.AddItem("{olive}", "橄榄色");
    menu.AddItem("{orange}", "橙色");
    menu.AddItem("{blue}", "蓝色");
    menu.AddItem("{purple}", "紫色");
    menu.Display(client, MENU_TIME_FOREVER);
}

public int NameColorMenuHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Select)
    {
        char color[MAX_COLOR_LENGTH];
        menu.GetItem(slot, color, sizeof(color));

        NCS_Cookie_Set(client, COOKIE_COLOR_NAME_KEY, color);
        NCS_Chat_SetNameColor(client, color);

        if (strlen(color) == 0) strcopy(color, sizeof(color), "{name}");

        NCS_Chat(client, _, "成功更换 %s名字颜色", color);
    }
    return 0;
}

void NameColor_UserInit(int client)
{
    // Wait for vip loaded
    CreateTimer(1.0, NameColor_Init, client, TIMER_FLAG_NO_MAPCHANGE);
}

public Action NameColor_Init(Handle timer, int client)
{
    if (!g_VIPs[client].IsValid())
        return Plugin_Stop;
    
    char color[MAX_COLOR_LENGTH];
    NCS_Cookie_Get(client, COOKIE_COLOR_NAME_KEY, color, sizeof(color));

    if (color[0] == '\0')
        return Plugin_Stop;
    
    if (g_VIPs[client].IsVIP())
        NCS_Chat_SetNameColor(client, color);
    else
        NCS_Cookie_Set(client, COOKIE_COLOR_NAME_KEY, "");
    
    return Plugin_Stop;
}