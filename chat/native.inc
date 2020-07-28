void InitNative()
{
    CreateNative("NCS_Chat_SetTitle", Native_SetTitle);
    CreateNative("NCS_Chat_SetNameColor", Native_SetNameColor);
}

public int Native_SetTitle(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    if (!player[client].IsValid())
        return 1;

    char title[MAX_TITLE_LENGTH], titleColor[MAX_COLOR_LENGTH];
    GetNativeString(2, title, sizeof(title));
    GetNativeString(3, titleColor, sizeof(titleColor));

    player[client].SetTitle(title, titleColor);
    return 1;
}

public int Native_SetNameColor(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    if (!player[client].IsValid())
        return 1;

    char nameColor[MAX_COLOR_LENGTH];
    GetNativeString(2, nameColor, sizeof(nameColor));

    player[client].SetNameColor(nameColor);
    return 1;
}