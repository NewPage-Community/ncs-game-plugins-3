void InitCmd()
{
	RegConsoleCmd("sm_store", Command_Store, "[NCS] 商店菜单");
	RegConsoleCmd("sm_shop", Command_Store, "[NCS] 商店菜单");
	RegConsoleCmd("sm_store_pass", Command_StorePass, "[NCS] 通行证商店菜单");
}

public Action Command_Store(int client, int args)
{
	LoadUserStoreList(client, DisplayStoreMenu);
	return Plugin_Handled;
}

public Action Command_StorePass(int client, int args)
{
	LoadUserStoreList(client);
	BuyAdvPassMenu(client, BuyAdvPassMenu);
	return Plugin_Handled;
}
