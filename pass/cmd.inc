void InitCmd()
{
	RegConsoleCmd("sm_pass", Command_PassMenu, "[NCS] 通行证菜单");
}

public Action Command_PassMenu(int client, int args)
{
	DisplayPassInfo(client);
	return Plugin_Handled;
}