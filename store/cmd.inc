void InitCmd()
{
	RegConsoleCmd("sm_store", Command_Store, "[NCS] 商店菜单");
	RegConsoleCmd("sm_shop", Command_Store, "[NCS] 商店菜单");
}

public Action Command_Store(int client, int args)
{
	LoadUserStoreList(client, true);
	return Plugin_Handled;
}
