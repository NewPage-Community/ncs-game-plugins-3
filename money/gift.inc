enum struct GiftData
{
	int Amount;
	int Target;
}

static GiftData playerGiftAmount[MAXPLAYERS + 1];

void GiftMoneyMenu(int client, int money)
{
	playerGiftAmount[client].Amount = money;

	Menu menu = new Menu(MoneyGiftMenuHandler);
	menu.SetTitle("赠送 %d 软妹币给:", money);
	for (int i = 1; i <= MaxClients; i++)
	{
		static char index[4], name[64];
		if (client == i) continue;
		if (!IsValidClient(i)) continue;
		IntToString(i, index, sizeof(index));
		GetClientName(i, name, sizeof(name));
		menu.AddItem(index, name);
	}
	menu.Display(client, MENU_TIME_FOREVER);
}

public int MoneyGiftMenuHandler(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_Cancel) delete menu;
	else if (action == MenuAction_Select)
	{
		static char index[4];
		menu.GetItem(slot, index, sizeof(index));
		playerGiftAmount[client].Target = StringToInt(index);
		MoneyGiftConfirmMenu(client);
	}
	return 0;
}

void MoneyGiftConfirmMenu(int client)
{
	Menu menu = new Menu(MoneyGiftConfirmMenuHandler);
	menu.SetTitle("确认赠送 %d 软妹币\n给 %N ?", playerGiftAmount[client].Amount, playerGiftAmount[client].Target);
	menu.AddItem("", "确认");
	menu.AddItem("", "算了");
	menu.Display(client, MENU_TIME_FOREVER);
}

public int MoneyGiftConfirmMenuHandler(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_Cancel) delete menu;
	else if (action == MenuAction_Select)
	{
		if (slot == 1) return 0;
		GiftMoney(client, playerGiftAmount[client].Target, playerGiftAmount[client].Amount);
	}
	return 0;
}