#define COOKIE_ALLCHAT_DISABLED_KEY "allchat_disabled"

bool GetCookieAllChatDisabled(int client)
{
    char type[8];
    // not set return false
    if (!NCS_Cookie_Get(client, COOKIE_ALLCHAT_DISABLED_KEY, type, sizeof(type)))
        return false;
    return StringToInt(type) == 1;
}

void SetCookieAllChatDisabled(int client, bool enable)
{
    char type[8];
    IntToString(enable, type, sizeof(type));
    NCS_Cookie_Set(client, COOKIE_ALLCHAT_DISABLED_KEY, type);
}
