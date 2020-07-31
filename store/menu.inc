static Menu playerLastMenu[MAXPLAYERS+1];
static int playerLastItem[MAXPLAYERS+1];

void DisplayStoreMenu(int client)
{
    LoadUserStoreList(client);
    Menu menu = new Menu(UserStoreListHandle); 
    menu.SetTitle("商店\n软妹币: %d", playerMoney[client]);
    menu.AddItem("0", "热门推荐")
    menu.AddItem("0", "升级通行证");
    menu.AddItem("1", "皮肤");
    menu.AddItem("2", "拖尾");
    menu.AddItem("3", "喷漆");
    menu.AddItem("4", "投掷物轨迹");
    menu.Display(client, MENU_TIME_FOREVER);
}

public int UserStoreListHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Select)
    {
        char type[8];
        menu.GetItem(slot, type, sizeof(type), _, _, _);

        if (StringToInt(type) == 0)
        {
            switch (slot)
            {
                case 0: HotSaleListMenu(client);
                case 1: BuyAdvPassMenu(client);
            }
        }
        else
            DisplayItemMenu(client, StringToInt(type));
    }
}

void DisplayItemMenu(int client, int type)
{
    if (!IsValidClient(client))
        return;
    
    Menu menu = new Menu(ItemMenuHandle);
    menu.SetTitle("购买%s\n软妹币: %d", typeName[type], playerMoney[client]);

    int length = playerStoreList[client].Length;
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        Item item;
        playerStoreList[client].Get(i, item);

        if (item.type != type)
            continue;

        char info[8], desc[64];
        item.GetMenuItem(info, sizeof(info), desc, sizeof(desc));
        menu.AddItem(info, desc);
        count++;
    }

    if (count == 0)
        menu.AddItem("", "货架空空如也～", ITEMDRAW_DISABLED);

    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);

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
        playerLastItem[client] = slot;

        char itemID[8];
        menu.GetItem(slot, itemID, sizeof(itemID), _, _, _);

        Item item;
        if (playerStoreList[client].Find(StringToInt(itemID), item) == -1)
            return;
        BuyItemMenu(client, item);
    }
}

void BuyItemMenu(int client, Item item)
{
    if (!IsValidClient(client))
        return;

    Menu menu = new Menu(BuyItemHandle);
    if (FloatCompare(1.0, item.discount) == 1)
        menu.SetTitle("购买 - %s\n现有软妹币: %d\n价格: %d(%1.1f折)\n ", item.name, playerMoney[client], item.price, item.discount * 10.0);
    else
        menu.SetTitle("购买 - %s\n现有软妹币: %d\n价格: %d\n ", item.name, playerMoney[client], item.price);

    char info[8], desc[64];
    IntToString(item.id, info, sizeof(info));
    Format(desc, sizeof(desc), "购买\n(支付 %d 软妹币)\n ", item.GetPrice());
    menu.AddItem(info, desc, (item.available && !item.alreadyHave && playerMoney[client] >= item.GetPrice()) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);

    switch (item.type)
    {
        case 1:
        {
            if (LibraryExists("NCS-Skin"))
                menu.AddItem(info, "预览模型\n(准星位置生成模型)");
        }
    }
    
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int BuyItemHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        Menu lastMenu = playerLastMenu[client];
        int lastPage = (playerLastItem[client]/lastMenu.Pagination)*lastMenu.Pagination;
        lastMenu.DisplayAt(client, lastPage, MENU_TIME_FOREVER);
    }
    else if (action == MenuAction_Select)
    {
        Menu lastMenu = playerLastMenu[client];
        int lastPage = (playerLastItem[client]/lastMenu.Pagination)*lastMenu.Pagination;
        char itemID[8];
        menu.GetItem(slot, itemID, sizeof(itemID), _, _, _);

        Item item;
        if (playerStoreList[client].Find(StringToInt(itemID), item) == -1)
            return;

        // Preview
        if (slot == 1)
        {
            switch (item.type)
            {
                case 1:
                    NCS_Skin_Preview(client, item.id);
            }
            BuyItemMenu(client, item);
        }
        else
        {
            BuyItem(client, item);
            lastMenu.DisplayAt(client, lastPage, MENU_TIME_FOREVER);
        }
    }
}

void HotSaleListMenu(int client)
{
    char path[128];
    Format(path, sizeof(path), "%s/hotsale", Store_Service);
    httpClient.Get(path, LoadHotSaleList, client);
}

public void LoadHotSaleList(HTTPResponse response, int client, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Store", "LoadHotSaleList", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

    if (!IsValidClient(client))
        return;

    // JSON data
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray array = view_as<JSONArray>(data.Get("items_id"));

    Menu menu = new Menu(ItemMenuHandle); 
    menu.SetTitle("热门推荐\n软妹币: %d", playerMoney[client]);
    
    int length = array.Length;
    for (int i = 0; i < length; i++)
    {
        Item item;
        if (playerStoreList[client].Find(array.GetInt(i), item) == -1)
            continue;

        char info[8], desc[64];
        item.GetMenuItem(info, sizeof(info), desc, sizeof(desc));
        menu.AddItem(info, desc);
    }

    if (length == 0)
        menu.AddItem("", "货架空空如也～", ITEMDRAW_DISABLED);

    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);

    playerLastMenu[client] = menu;

    delete array;
}

void BuyAdvPassMenu(int client)
{
    Menu menu = new Menu(BuyAdvPassHandle);
    menu.SetTitle("升级通行证\n软妹币: %d", playerMoney[client]);

    for (int i = 1; i < sizeof(passTypeName); i++)
    {
        char info[8], desc[64];
        IntToString(i, info, sizeof(info));
        Format(desc, sizeof(desc), "%s\n(%d 软妹币)", passTypeName[i], passTypePrice[i]);
        menu.AddItem(info, desc);
    }
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);

    playerLastMenu[client] = menu;
}

public int BuyAdvPassHandle(Menu menu, MenuAction action, int client, int slot)
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
        char type[8];
        menu.GetItem(slot, type, sizeof(type), _, _, _);
        BuyAdvPassConfirm(client, StringToInt(type));
    }
}

void BuyAdvPassConfirm(int client, int type)
{
    Menu menu = new Menu(BuyAdvPassConfirmHandle);
    menu.SetTitle("通行证升级确认\n软妹币: %d\n \n内容:\n%s\n \n两种升级方案只能 选择一种\n ", playerMoney[client], passTypeDesc[type]);
    
    char info[8], desc[64];
    IntToString(type, info, sizeof(info));
    Format(desc, sizeof(desc), "购买%s\n(支付 %d 软妹币)", passTypeName[type], passTypePrice[type]);
    menu.AddItem(info, desc, playerPass[client] == 0 && playerMoney[client] >= passTypePrice[type] ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int BuyAdvPassConfirmHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        Menu lastMenu = playerLastMenu[client];
        int lastPage = (playerLastItem[client]/lastMenu.Pagination)*lastMenu.Pagination;
        lastMenu.DisplayAt(client, lastPage, MENU_TIME_FOREVER);
    }
    else if (action == MenuAction_Select)
    {
        char type[8];
        menu.GetItem(slot, type, sizeof(type), _, _, _);
        BuyAdvPass(client, StringToInt(type));
        DisplayStoreMenu(client);
    }
}