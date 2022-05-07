void InitCmd()
{
	RegConsoleCmd("sm_vip", Command_VIP, "[NCS] VIP菜单");
}

public Action Command_VIP(int client, int args)
{
	DisplayVIPMenu(client);
	return Plugin_Handled;
}
