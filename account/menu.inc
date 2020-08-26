void AccountMenu(int client)
{
    if (!g_Accounts[client].Valid())
    {
        NCS_Chat(client, _, "{blue}您的用户数据未加载");
        return;
    }

    char steam2[32], steam3[32], steamid64[32], ip[32];
    GetClientAuthId(client, AuthId_Steam2, steam2, sizeof(steam2), true);
    GetClientAuthId(client, AuthId_Steam3, steam3, sizeof(steam3), true);
    GetClientIP(client, ip, sizeof(ip));

    Panel panel = CreatePanel();

    panel.SetTitle("个人信息");
    DrawPanelTextEx(panel, " ");
    DrawPanelTextEx(panel, "玩家名: %d", g_Accounts[client].name);
    DrawPanelTextEx(panel, "社区个人唯一身份 (UID): %d", g_Accounts[client].uid);
    DrawPanelTextEx(panel, "STEAM2 ID: %s", steam2);
    DrawPanelTextEx(panel, "STEAM3 ID: %s", steam3);
    DrawPanelTextEx(panel, "STEAMID 64: %s", g_Accounts[client].steamid);
    DrawPanelTextEx(panel, "IP地址: %s", ip);
    DrawPanelTextEx(panel, " ");
    panel.DrawItem("退出");

    panel.Send(client, MenuHandler_InfoPanel, 0);
}

public int MenuHandler_InfoPanel(Handle menu, MenuAction action, int client, int itemNum)
{
    if(action == MenuAction_End)
        delete menu;
}