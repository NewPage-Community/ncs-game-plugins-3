void DisplayKickTargetMenu(int client)
{
	if (!g_VIPs[client].IsValid())
        return;

	Menu menu = new Menu(MenuHandler_Kick);
	
	char title[100];
	Format(title, sizeof(title), "%T:", "Kick vote", client);
	menu.SetTitle(title);
	
	AddTargetsToMenu(menu, 0, false, false);
	
	menu.Display(client, MENU_TIME_FOREVER);
}

public int MenuHandler_Kick(Menu menu, MenuAction action, int param1, int param2)
{
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		char info[32], name[32];
		int userid, target;
		
		menu.GetItem(param2, info, sizeof(info), _, name, sizeof(name));
		userid = StringToInt(info);

		if ((target = GetClientOfUserId(userid)) == 0)
		{
			PrintToChat(param1, "[SM] %t", "Player no longer available");
		}
		else if (!CanUserTarget(param1, target))
		{
			PrintToChat(param1, "[SM] %t", "Unable to target");
		}
		else
		{
			ServerCommand("sm_votekick #%d", userid);
		}
	}
}