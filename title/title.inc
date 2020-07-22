#include <sdktools>
#include <ncs/vip>
#include <ncs/chat>

enum struct TitleData
{
    char title[MAX_NAME_LENGTH];
    int type;
    bool isWaitingForplayerTitle;
}

TitleData playerTitle[MAX_NAME_LENGTH+1];

#include "title/utils"
#include "title/api"
#include "title/menu"

void TitleBeSet(int client, int type)
{
    if (!IsValidClient(client))
        return;

    char playerName[MAX_NAME_LENGTH];
    GetClientName(client, playerName, sizeof(playerName));

    switch (type)
    {
        case 0:
        {
            return;
        }
        case 1:
        {
            if (NCS_VIP_IsVIP(client))
                Format(playerName, sizeof(playerName),"<%s> %s", playerTitle[client].title, playerName);
        }
        case 2:
        {
            if (NCS_VIP_IsVIP(client))
                Format(playerName, sizeof(playerName),"<VIP> %s", playerName);
        }
        case 3:
        {
            if (IsOwner(client))
                Format(playerName, sizeof(playerName),"<服主> %s", playerName);
        }
        case 4:
        {
            if (IsAdmin(client))
                Format(playerName, sizeof(playerName),"<管理> %s", playerName);
        }
        default:
        {
            return;
        }
    }
    
    NCS_Account_ChangeName(client, playerName);
}

void SetplayerTitle(int client, const char[] title)
{
    strcopy(playerTitle[client].title, MAX_NAME_LENGTH, title);
    TitleBeSet(client, 1);
    // Save
    SetTitle(client);
}
