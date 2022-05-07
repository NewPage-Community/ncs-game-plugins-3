void AccountMenu(int client)
{
	if (!g_Accounts[client].Valid())
	{
		NCS_Chat(client, _, "您的用户数据未加载");
		return;
	}

	char steam2[32], steam3[32], ip[32];
	GetClientAuthId(client, AuthId_Steam2, steam2, sizeof(steam2));
	GetClientAuthId(client, AuthId_Steam3, steam3, sizeof(steam3));
	GetClientIP(client, ip, sizeof(ip));

	PanelEx panel = new PanelEx();

	panel.SetTitle("个人信息");
	panel.DrawTextEx(" ");
	panel.DrawTextEx("玩家名: %s", g_Accounts[client].name);
	panel.DrawTextEx("社区个人唯一身份 (UID): %s", g_Accounts[client].uid);
	panel.DrawTextEx("STEAM2 ID: %s", steam2);
	panel.DrawTextEx("STEAM3 ID: %s", steam3);
	panel.DrawTextEx("STEAMID 64: %s", g_Accounts[client].steamid);
	panel.DrawTextEx("IP地址: %s", ip);
	panel.DrawTextEx(" ");
	panel.DrawItem("退出");

	panel.Send(client, MenuHandler_InfoPanel, 0);
}

public int MenuHandler_InfoPanel(Handle menu, MenuAction action, int client, int itemNum)
{
	if(action == MenuAction_End)
		delete menu;
	return 0;
}