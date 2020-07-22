void InitCmd()
{
	RegConsoleCmd("sm_store", Command_Store, "[NCS] 商店菜单");
}

public Action Command_Store(int client, int args)
{
	DisplayStoreMenu(client);
	return Plugin_Handled;
}
