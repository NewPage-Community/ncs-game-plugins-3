void InitPlayerTitle(int client)
{
    if (!IsValidClient(client))
        return;

    player[client].type = GetCookieTitleType(client);
    GetCookieTitleCustom(client, player[client].title, MAX_TITLE_LENGTH);
    player[client].isWaitingForPlayerTitle = false;
    CreateTimer(1.0, SetTitleFirst, client);
}

public Action SetTitleFirst(Handle timer, int client)
{
    SetPlayerTitle(client);
    return Plugin_Stop;
}

void SetPlayerTitle(int client)
{
    if (!IsValidClient(client))
        return;
    
    char title[MAX_TITLE_LENGTH], titleColor[MAX_COLOR_LENGTH];

    switch (player[client].type)
    {
        case 0: {}
        case 1:
        {
            if (NCS_VIP_IsVIP(client) && NCS_VIP_Level(client) >= 100)
            {
                Format(title, sizeof(title), "<%s>", player[client].title);
                strcopy(titleColor, sizeof(titleColor), "{blue}");
            }
        }
        case 2:
        {
            if (NCS_VIP_IsVIP(client))
            {
                strcopy(title, sizeof(title), "<VIP>");
                strcopy(titleColor, sizeof(titleColor), "{red}");
            }
        }
        case 3,4:
        {
            if (IsOwner(client) || IsAdmin(client))
            {
                strcopy(title, sizeof(title), "<管理>");
                strcopy(titleColor, sizeof(titleColor), "{green}");
            }
        }
        default: {}
    }
    
    NCS_Chat_SetTitle(client, title, titleColor);
}

void UpdatePlayerTitleType(int client, int type)
{
    // Cookie
    SetCookieTitleType(client, type);

    // Game
    player[client].type = type;
    SetPlayerTitle(client);
}

void UpdatePlayerTitleCustom(int client, const char[] buffer)
{
    // Cookie
    SetCookieTitleCustom(client, buffer);

    // Game
    strcopy(player[client].title, MAX_TITLE_LENGTH, buffer);
    SetPlayerTitle(client);
}