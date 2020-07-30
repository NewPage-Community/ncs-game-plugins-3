static Menu playerLastMenu[MAXPLAYERS+1];
static int playerMoney[MAXPLAYERS+1];

void DisplayStoreMenu(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;
    
    char path[128];
    Format(path, sizeof(path), "%s/list/%s", Store_Service, uid);
    httpClient.Get(path, LoadUserStoreList, client);
}

public void LoadUserStoreList(HTTPResponse response, int client, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Store", "LoadUserStoreList", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

    if (!IsValidClient(client))
        return;

    // JSON data
    JSONObject data = view_as<JSONObject>(response.Data);
    if (playerStoreList[client] != INVALID_HANDLE)
        delete playerStoreList[client];
    playerStoreList[client] = new Items(view_as<JSONArray>(data.Get("items")));
    playerMoney[client] = data.GetInt("money");

    Menu menu = new Menu(UserStoreListHandle); 
    menu.SetTitle("商店\n软妹币:%d", playerMoney[client]);
    menu.AddItem("1", "皮肤");
    menu.AddItem("2", "拖尾");
    menu.AddItem("3", "喷漆");
    menu.AddItem("4", "投掷物轨迹");
    menu.Display(client, 60);
}

public int UserStoreListHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Select)
    {
        DisplayItemMenu(client, slot+1)
    }
}

void DisplayItemMenu(int client, int type)
{
    if (!IsValidClient(client))
        return;
    
    Menu menu = new Menu(ItemMenuHandle);
    menu.SetTitle("购买%s\n软妹币:%d", typeName[type], playerMoney[client]);

    int length = playerStoreList[client].Length;
    for (int i = 0; i < length; i++)
    {
        Item item;
        playerStoreList[client].Get(i, item);

        if (item.type != type)
            continue;

        char info[8], desc[64];
        IntToString(item.id, info, sizeof(info));
        if (item.discount < 1.0)
            Format(desc, sizeof(desc), "%s (%f1.1折)", item.name, item.discount * 10.0);
        else
            Format(desc, sizeof(desc), "%s", item.name);
        menu.AddItem(info, desc);
    }

    menu.ExitBackButton = true;
    menu.Display(client, 60);

    playerLastMenu[client] = menu;
}

public int ItemMenuHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End && slot == MenuEnd_Exit)
    {
        delete menu;
    }
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        DisplayStoreMenu(client);
        delete menu;
    }
    else if (action == MenuAction_Select)
    {
        char itemID[8];
        menu.GetItem(slot, itemID, sizeof(itemID), _, _, _);

        Item item;
        int index = playerStoreList[client].Find(StringToInt(itemID));
        if (index == 0)
            return;
        playerStoreList[client].Get(index, item);
        BuyItemMenu(client, item);
    }
}

void BuyItemMenu(int client, Item item)
{
    if (!IsValidClient(client))
        return;

    Menu menu = new Menu(BuyItemHandle);
    if (item.discount < 1.0)
        menu.SetTitle("购买 - %s\n现有软妹币:%d\n价格:%d(%f1.1折)", item.name, playerMoney[client], item.price, item.discount * 10.0);
    else
        menu.SetTitle("购买 - %s\n现有软妹币:%d\n价格:%d", item.name, playerMoney[client], item.price);

    char info[8], desc[64];
    IntToString(item.id, info, sizeof(info));
    Format(desc, sizeof(desc), "购买\n(支付 %d 软妹币)", item.GetPrice());
    menu.AddItem(info, desc, (item.available && !item.alreadyHave && playerMoney[client] >= item.GetPrice()) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);

    switch (item.type)
    {
        case 1:
            if (LibraryExists("NCS-Skin"))
                menu.AddItem(info, "预览模型(准星位置生成模型)");
    }
    
    menu.ExitBackButton = true;
    menu.Display(client, 60);
}

public int BuyItemHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
        playerLastMenu[client].Display(client, 60);
    else if (action == MenuAction_Select)
    {
        char itemID[8];
        menu.GetItem(slot, itemID, sizeof(itemID), _, _, _);

        Item item;
        int index = playerStoreList[client].Find(StringToInt(itemID));
        if (index == 0)
            return;

        // Preview
        if (slot == 1)
        {
            switch (item.type)
            {
                case 1:
                    NCS_Skin_Preview(client, item.id);
            }
            menu.Display(client, 60);
        }
        else
            BuyItem(client, item);
    }
}