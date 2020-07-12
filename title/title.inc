#include "title/api"
#include <sdktools>
#include <ncs/account>


void Title_set(int client,int customType,char customTitle){
    char playerName[MAX_NAME_LENGTH];
    char playerNameWithTitle[MAX_NAME_LENGTH];

    switch (customType)
    {
        case 1:
        {
            return;
        }
        case 2, 3:
        {
            GetClientName(client, playerName, sizeof(playerName));
            Format(playerNameWithTitle, sizeof(playerNameWithTitle),"[%s] %s",customTitle,playerName);
            SetClientName(client, playerNameWithTitle);
        }
        default:
        {
            return;
        }
    }

}

Action Set_Title(int client, int args){
    char arg[MAX_NAME_LENGTH];

    if (args != 1)
    {
        return Plugin_Handled;
    }

    if (!IsValidClient(client)){
        return Plugin_Handled;
    }
        
    GetCmdArg(1, arg, sizeof(arg));
    SetTitle(client,arg);

    return Plugin_Handled;

}


