static Menu playerLastMenu[MAXPLAYERS+1];
static int playerLastItem[MAXPLAYERS+1];

void DisplayStoreMenu(int client)
{
    Menu menu = new Menu(UserStoreListHandle); 
    menu.SetTitle("商店\n软妹币: %d", playerMoney[client]);
    menu.AddItem("0", "热门推荐")
    menu.AddItem("0", "升级通行证");
    menu.AddItem("0", "续费VIP");
    menu.AddItem("1", "皮肤");
    menu.AddItem("2", "足迹");
    menu.AddItem("3", "光环");
    menu.AddItem("4", "喷漆");
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
                case 2: BuyVIPMenu(client);
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

    StringMapSnapshot ids = playerStoreList[client].Snapshot();
    int length = ids.Length;
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        // Get item id
        char id[8];
        ids.GetKey(i, id, sizeof(id));

        // Get item info
        Item item;
        playerStoreList[client].CGet(id, item);

        if (item.type != type)
            continue;

        char info[8], desc[64];
        item.GetMenuItem(info, sizeof(info), desc, sizeof(desc));
        if (!item.available)
            Format(desc, sizeof(desc), "%s\n（限定物品）", desc);
        menu.AddItem(info, desc);
        count++;
    }
    delete ids;

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
        if (!playerStoreList[client].CGet(itemID, item))
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
        menu.SetTitle("购买 - %s\n现有软妹币: %d\n价格: %d(%1.1f折)\n \n%s\n ", item.name, playerMoney[client], item.price, item.discount * 10.0, item.description);
    else
        menu.SetTitle("购买 - %s\n现有软妹币: %d\n价格: %d\n \n%s\n ", item.name, playerMoney[client], item.price, item.description);

    char info[8], desc[64];
    IntToString(item.id, info, sizeof(info));
    if (item.available)
    {
        if (item.alreadyHave)
            Format(desc, sizeof(desc), "已拥有\n ");
        else
            Format(desc, sizeof(desc), "购买\n(支付 %d 软妹币)\n ", item.GetPrice());
    }
    else
        Format(desc, sizeof(desc), "不可购买\n ");
    menu.AddItem(info, desc, (item.available && !item.alreadyHave && playerMoney[client] >= item.GetPrice()) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);

    switch (item.type)
    {
        case 1:
        {
            if (LibraryExists("NCS-Skin"))
                menu.AddItem(info, "预览模型\n(准星位置生成)");
        }
        case 2:
        {
            if (LibraryExists("NCS-Trail"))
                menu.AddItem(info, "预览足迹\n(玩家活着有效)");
        }
        case 3:
        {
            if (LibraryExists("NCS-Aura"))
                menu.AddItem(info, "预览光环\n(玩家活着有效)");
        }
        case 4:
        {
            if (LibraryExists("NCS-Spray"))
                menu.AddItem(info, "预览喷漆\n(准星位置生成)");
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
        if (!playerStoreList[client].CGet(itemID, item))
            return;

        // Preview
        if (slot == 1)
        {
            switch (item.type)
            {
                case 1:
                    NCS_Skin_Preview(client, item.id);
                case 2:
                    NCS_Trail_Preview(client, item.id);
                case 3:
                    NCS_Aura_Preview(client, item.id);
                case 4:
                    NCS_Spray_Preview(client, item.id);
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
    if (CheckAPIError("Store", "LoadHotSaleList", response, error))
        return;

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
        if (!playerStoreList[client].Get(array.GetInt(i), item))
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
    MenuEx menu = new MenuEx(BuyAdvPassHandle);
    menu.SetTitle("升级通行证\n软妹币: %d", playerMoney[client]);

    for (int i = 1; i < sizeof(passTypeName); i++)
    {
        char info[8];
        IntToString(i, info, sizeof(info));
        menu.AddItemEx(0, info, "%s\n(%d 软妹币)", passTypeName[i], passTypePrice[i]);
    }

    // pass point box
    Item box;
    playerStoreList[client].Get(1000, box);
    char info[8], desc[64];
    box.GetMenuItem(info, sizeof(info), desc, sizeof(desc));
    menu.AddItem(info, desc);

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

        int itype = StringToInt(type);
        if (itype == 1000)
        {
            Item item;
            if (!playerStoreList[client].CGet("1000", item))
                return;
            BuyItemMenu(client, item);
        }
        else if (itype != 0)
            BuyAdvPassConfirm(client, itype);
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

void BuyVIPMenu(int client)
{
    MenuEx menu = new MenuEx(BuyVIPMenuHandle);
    menu.SetTitle("续费VIP\n软妹币: %d", playerMoney[client]);

    menu.AddItemEx(0, "1", "1个月\n(%d 软妹币)", VIPPrice(1));
    menu.AddItemEx(0, "3", "3个月\n(%d 软妹币)", VIPPrice(3));
    menu.AddItemEx(0, "6", "6个月\n(%d 软妹币)", VIPPrice(6));
    menu.AddItemEx(0, "12", "12个月\n(%d 软妹币)", VIPPrice(12));

    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);

    playerLastMenu[client] = menu;
}

public int BuyVIPMenuHandle(Menu menu, MenuAction action, int client, int slot)
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
        char month[8];
        menu.GetItem(slot, month, sizeof(month), _, _, _);
        BuyVIPConfirm(client, StringToInt(month))
    }
}

void BuyVIPConfirm(int client, int month)
{
    Menu menu = new Menu(BuyVIPConfirmHandle);
    
    if (LibraryExists("NCS-VIP"))
    {
        char expireTime[INT64_LENGTH], expireDate[256];
        NCS_VIP_ExpireTime(client, expireTime, sizeof(expireTime));
        if (StringToInt(expireTime) == 0)
            Format(expireDate, sizeof(expireDate), "未开通");
        else
            FormatTime(expireDate, sizeof(expireDate), NULL_STRING, StringToInt(expireTime));
        menu.SetTitle("VIP续费确认\n软妹币: %d\n有效期值: %s\n \n%s\n ", playerMoney[client], expireDate, vipDesc);
    }
    else
    {
        menu.SetTitle("VIP续费确认\n软妹币: %d\n \n%s\n ", playerMoney[client], vipDesc);
    }
    
    int price = VIPPrice(month);
    char info[8], desc[64];
    IntToString(month, info, sizeof(info));
    Format(desc, sizeof(desc), "续费%d个月\n(支付 %d 软妹币)", month, price);
    menu.AddItem(info, desc, playerMoney[client] >= price ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int BuyVIPConfirmHandle(Menu menu, MenuAction action, int client, int slot)
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
        char month[8];
        menu.GetItem(slot, month, sizeof(month), _, _, _);
        BuyVIP(client, StringToInt(month));
        DisplayStoreMenu(client);
    }
}