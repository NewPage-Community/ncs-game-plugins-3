enum struct TitleData
{
    char title[MAX_TITLE_LENGTH];
    int type;
    bool isWaitingForplayerTitle;
}

TitleData playerTitle[MAXPLAYERS+1];

#include "title/utils"
#include "title/api"
#include "title/menu"

void TitleBeSet(int client, int type)
{
    if (!IsValidClient(client))
        return;

    char playerName[MAX_NAME_LENGTH_FIX], title[MAX_TITLE_LENGTH], titleColor[MAX_COLOR_LENGTH];
    NCS_Account_GetName(client, playerName, sizeof(playerName));

    switch (type)
    {
        case 0:
        {
            return;
        }
        case 1:
        {
            if (NCS_VIP_IsVIP(client) && NCS_VIP_Level(client) >= 100)
            {
                Format(title, sizeof(title), "<%s>", playerTitle[client].title);
                strcopy(titleColor, sizeof(titleColor), "{blue}");
                Format(playerName, sizeof(playerName),"%s %s", title, playerName);
            }
        }
        case 2:
        {
            if (NCS_VIP_IsVIP(client))
            {
                strcopy(title, sizeof(title), "<VIP>");
                strcopy(titleColor, sizeof(titleColor), "{red}");
                Format(playerName, sizeof(playerName),"%s %s", title, playerName);
            }
        }
        case 3:
        {
            if (IsOwner(client))
            {
                strcopy(title, sizeof(title), "<服主>");
                strcopy(titleColor, sizeof(titleColor), "{purple}");
                Format(playerName, sizeof(playerName),"%s %s", title, playerName);
            }
        }
        case 4:
        {
            if (IsAdmin(client))
            {
                strcopy(title, sizeof(title), "<管理>");
                strcopy(titleColor, sizeof(titleColor), "{green}");
                Format(playerName, sizeof(playerName),"%s %s", title, playerName);
            }
        }
        default:
        {
            return;
        }
    }
    
    NCS_Account_ChangeName(client, playerName);
    NCS_Chat_SetTitle(client, title, titleColor);
}

void SetplayerTitle(int client, const char[] title)
{
    strcopy(playerTitle[client].title, MAX_TITLE_LENGTH, title);
    TitleBeSet(client, 1);
    // Save
    SetTitle(client);
}
