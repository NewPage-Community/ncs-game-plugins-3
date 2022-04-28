void InitCmd()
{
    RegConsoleCmd("sm_prefix", Command_Prefix, "[NCS] 头衔设置");
    RegConsoleCmd("sm_title", Command_Prefix, "[NCS] 头衔设置");
}

public Action Command_Prefix(int client, int args)
{
    if (!IsValidClient(client))
        return Plugin_Handled;

    player[client].isWaitingForPlayerTitle = false;

    MenuEx menu = new MenuEx(MenuHandler_Prefix);
    menu.SetTitle("请选择你显示的头衔");

    menu.AddItemEx(0, "0", "不显示");
    if (NCS_VIP_IsVIP(client) && NCS_VIP_Level(client) >= 40)
    {
        menu.AddItemEx(0, "-1", "设置自定义头衔");
        if (player[client].title[0] != '\0')
            menu.AddItemEx(0, "1", "<%s>", player[client].title);
    }
    if (NCS_VIP_IsVIP(client))
        menu.AddItemEx(0, "2", "<VIP>");
    if (IsOwner(client))
        menu.AddItemEx(0, "3", "<服主>");
    if (IsAdmin(client))
        menu.AddItemEx(0, "4", "<管理>");

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
            player[client].isWaitingForPlayerTitle = true;
            NCS_Chat(client, _, "请在聊天栏输入你想设置的自定义头衔");
        }
        else
        {
            UpdatePlayerTitleType(client, itype);
        }
    }

    return 0;
}