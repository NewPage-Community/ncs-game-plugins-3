void InitCmd()
{
    RegConsoleCmd("sm_prefix", Command_Prefix, "sm_prefix <prefix>");
}

public Action Command_Prefix(int client, int args)
{
    if (!IsValidClient(client))
        return Plugin_Handled;

    playerTitle[client].isWaitingForplayerTitle = false;

    Menu menu = new Menu(MenuHandler_Prefix);
    menu.SetTitle("请选择你显示的头衔");

    AddMenuItemEx(menu, 0, "0", "不显示");
    if (NCS_VIP_IsVIP(client))
    {
        AddMenuItemEx(menu, 0, "-1", "设置自定义头衔");
        if (playerTitle[client].title[0] != '\0')
            AddMenuItemEx(menu, 0, "1", "<%s>", playerTitle[client].title);
    }
    if (NCS_VIP_IsVIP(client))
        AddMenuItemEx(menu, 0, "2", "<会员>");
    if (IsOwner(client))
        AddMenuItemEx(menu, 0, "3", "<服主>");
    if (IsAdmin(client))
        AddMenuItemEx(menu, 0, "4", "<管理>");

    menu.ExitButton = true;
    menu.Display(client, 0);
    return Plugin_Handled;
}

public int MenuHandler_Prefix(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_Cancel)
    {
        if(slot == MenuCancel_ExitBack)
            delete menu;
    }
    else if (action == MenuAction_Select)
    {
        char type[8];
        menu.GetItem(slot, type, sizeof(type), _, _, _);
        int itype = StringToInt(type);
        if (itype == -1)
        {
            playerTitle[client].isWaitingForplayerTitle = true;
            NCS_Chat(client, _, "{blue}请在聊天栏输入你想设置的自定义头衔！");
        }
        else
        {
            playerTitle[client].type = itype;
            TitleBeSet(client, itype);

            // Save
            SetTitle(client);
        }
    }
}