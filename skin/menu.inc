#define Backpack_Service "backpack/user"

int playerLast[MAXPLAYERS + 1];
int playerLastSkin[MAXPLAYERS + 1];

void DisplayUserSkins(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
    {
        NCS_Chat(client, _, "您的用户数据尚未加载，请重试");
        return;
    }

    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);

    char path[128];
    Format(path, sizeof(path), "%s/%s", Backpack_Service, uid);
    NewNCSAPI(path).Get(UserSkinsCallback, dp);
}

public void UserSkinsCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    if (CheckAPIError("Skin", "UserSkinsCallback", response, error))
        return;

    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    char uid[MAX_UID_LENGTH];
    dp.ReadString(uid, sizeof(uid));
    dp.Close();

    // JSON
    char iUID[MAX_UID_LENGTH];
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray items = view_as<JSONArray>(data.Get("items"));
    int length = items.Length;

    // Check UID is valid
    data.GetString("uid", iUID, sizeof(iUID));
    if (!StrEqual(iUID, uid) || !IsValidClient(client))
    {
        delete items;
        return;
    }
    
    // Menu

    Menu menu = new Menu(Menu_SkinSelected);
    menu.ExitButton = true;
    menu.SetTitle("选择你要使用的皮肤");
    menu.AddItem("0", "默认皮肤");

    for (int i = 0; i < length; i++)
    {
        JSONObject item = view_as<JSONObject>(items.Get(i));
        char skinID[8];
        char exprTime[INT64_LENGTH];
        IntToString(item.GetInt("id"), skinID, sizeof(skinID));
        item.GetString("expr_time", exprTime, sizeof(exprTime));

        Skin s;
        if (!g_Skins.Get(item.GetInt("id"), s))
        {
            delete item;
            continue;
        }
        delete item;
        
        char desc[64];
        int left = StringToInt(exprTime) - GetTime();
        if (left > 0)
            Format(desc, sizeof(desc), "%s(%d天%d小时)", s.name, left / 86400, (left % 86400) / 3600);
        else
            Format(desc, sizeof(desc), "%s", s.name);
        bool used = g_UsersUsedSkin[client] == s.id;
        if (used)
            Format(desc, sizeof(desc), "(使用中)%s", desc);
        menu.AddItem(skinID, desc, used ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);
    }

    if (playerLast[client]) menu.DisplayAt(client, playerLast[client], MENU_TIME_FOREVER);
    else menu.Display(client, MENU_TIME_FOREVER);

    delete items;
}

public int Menu_SkinSelected(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
    {
        delete menu;
    }
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        delete menu;
    }
    else if (action == MenuAction_Select)
    {
        if (IsValidClient(client))
        {
            playerLast[client] = menu.Selection;
            char skinID[32];
            menu.GetItem(slot, skinID, sizeof(skinID));
            DisplaySkinMenu(client, StringToInt(skinID));
        }
    }
    return 0;
}

void DisplaySkinMenu(int client, int skinID)
{
    if (!skinID) return;
    playerLastSkin[client] = skinID;

    static Skin skin;
    if (!g_Skins.Get(skinID, skin)) return;

    bool used = g_UsersUsedSkin[client] == skin.id
    MenuEx menu = new MenuEx(Menu_Skin);
    menu.SetTitle("%s - 皮肤设置\n", skin.name);
    menu.AddItemEx(used ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT, "", "装备皮肤%s", used ? "(使用中)" : "");
    menu.AddItem("", "组件设置", !skin.bodys.Length ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);
    menu.Display(client, MENU_TIME_FOREVER);
}

public int Menu_Skin(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
    {
        delete menu;
    }
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
    {
        delete menu;
    }
    else if (action == MenuAction_Select)
    {
        if (IsValidClient(client))
        {
            int skinID = playerLastSkin[client];
            if (slot == 0)
            {
                static Skin skin;
                if (!g_Skins.Get(skinID, skin)) return 0;
                SetUsedSkin(client, skinID);
#if defined GAME_INSURGENCY
                SetPlayerModelExt(client, skinID);
#else
                SetModel(client);
#endif
                NCS_Chat(client, _, "已成功更换为 {lime}%s{default}，可通过 {olive}!tp{default} 查看模型", skin.name);
            }
            else if (slot == 1)
            {
                SetPlayerModelExt(client, skinID);
            }
        }
    }
    return 0;
}

void SkinBodyMenu(int client, int skinID)
{
    static BodysData body;
    Skin skin;
    if (!g_Skins.Get(skinID, skin)) return;

    Menu menu = CreateMenu(SkinBodyMenuHandler);
    menu.SetTitle("%s - 组件设置", skin.name);

    static char info[128];
    menu.AddItem("", "重置");
    for (int i = 0; i < skin.bodys.Length; i++) {
        if (!skin.bodys.GetArray(i, body)) ThrowError("GetArray");
#if defined GAME_INSURGENCY
        CBaseAnimating animating = CBaseAnimating(client);
        int value = animating.GetBodyGroup(GetEntProp(client, Prop_Send, "m_nBody"), body.index);
        Format(info, sizeof(info), "%s: 形态%d", body.name, value + 1);
#endif
        menu.AddItem("", info);
    }
    if (playerLast[client]) menu.DisplayAt(client, playerLast[client], MENU_TIME_FOREVER);
    else menu.Display(client, MENU_TIME_FOREVER);
}

int SkinBodyMenuHandler(Menu menu, MenuAction action, int client, int param) {
    if (action == MenuAction_End) menu.Close();
    else if (action == MenuAction_Select)
    {
        if (param == MenuCancel_ExitBack)
        {
            DisplayUserSkins(client);
            return 0;
        }

        playerLast[client] = menu.Selection;
        static Skin skin;
        if (!g_Skins.Get(playerLastSkin[client], skin)) return 0;
        if (param == 0)
        {
            NCS_Chat(client, _, "组件已重置");
            SetUserSkinBody(client, playerLastSkin[client], 0);
        }
        else 
        {
            static BodysData body;
            if (!skin.bodys.GetArray(param - 1, body)) ThrowError("GetArray");
#if defined GAME_INSURGENCY
            CBaseAnimating animating = CBaseAnimating(client);
            int value = animating.GetBodyGroup(GetEntProp(client, Prop_Send, "m_nBody"), body.index) + 1;
            value %= animating.GetBodyGroupCount(body.index);
            animating.SetBodyGroup(body.index, value);
            SetPlayerModelExt(client, playerLastSkin[client]);
#endif
            SetUserSkinBody(client, playerLastSkin[client], GetEntProp(client, Prop_Send, "m_nBody"));
            NCS_Chat(client, _, "组件 {lime}%s{default} 已设置为 {lime}形态%d", body.name, value + 1);
        }
        SkinBodyMenu(client, playerLastSkin[client]);
    }
    return 0;
}