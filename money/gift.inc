static int playerGiftAmount[MAXPLAYERS + 1];

void GiftMoneyMenu(int client, int money)
{
    playerGiftAmount[client] = money;

    Menu menu = new Menu(MoneyGiftMenuHandler);
    menu.SetTitle("赠送 %d 软妹币给:", money);
    for (int i = 1; i <= MaxClients; i++)
    {
        static char index[4], name[64];
        if (!IsValidClient(i)) continue;
        IntToString(i, index, sizeof(index));
        GetClientName(client, name, sizeof(name));
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
        int target = StringToInt(index);
        GiftMoney(client, target, playerGiftAmount[client]);
    }
    return 0;
}