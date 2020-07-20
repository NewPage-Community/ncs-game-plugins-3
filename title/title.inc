#include "title/api"
#include <sdktools>
#include <ncs/account>
#include <ncs/vip>
// #include <ncs/menu>


void TitleBeSet(int client, int type){

    if (!IsValidClient(client)){
        return ;
    }

    char playerName[MAX_NAME_LENGTH];
    char playerNameWithTitle[MAX_NAME_LENGTH];

    switch (type)
    {
        case 1:
        {
            return;
        }
        case 2, 3:
        {
            GetClientName(client, playerName, sizeof(playerName));
            Format(playerNameWithTitle, sizeof(playerNameWithTitle),"[%s] %s",customTitle[client].title,playerName);
            NCS_Account_ChangeName(client, playerNameWithTitle);
        }
        default:
        {
            return;
        }
    }

}

// public Action Set_Title(int client, int args){
//     char arg[MAX_NAME_LENGTH];

//     if (args != 1)
//     {
//         return Plugin_Handled;
//     }

//     if (!IsValidClient(client)){
//         return Plugin_Handled;
//     }
        
//     GetCmdArg(1, arg, sizeof(arg));
//     SetTitle(client,arg);


//     return Plugin_Handled;

// }

public int MenuHandler_Prefix(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_Cancel)
	{
		if(slot == MenuCancel_ExitBack)
			delete menu;
	}
	else if (action == MenuAction_Select)
	{
		char type[2];
		menu.GetItem(slot, type, 2, _, _, _);
		TitleBeSet(client,StringToInt(type));
	}
}

public Action Command_Prefix(int client, int args)
{
    if (!IsValidClient(client))
        return Plugin_Handled;

    Menu menu = new Menu(MenuHandler_Prefix);
    menu.SetTitle("请选择你显示的头衔");

    char title[MAX_NAME_LENGTH];
    title = customTitle[client].title ;

    AddMenuItemEx(menu, 0, "1", "不显示");
    if (NCS_VIP_IsVIP(client)){
        if (title[0] != '\0'){
            AddMenuItemEx(menu, 0, "2", "<%s>", title);
        }
    }

    menu.ExitButton = true;
    menu.Display(client, 0);
    return Plugin_Handled;
}
