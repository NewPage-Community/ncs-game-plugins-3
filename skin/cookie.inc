void GetUsedSkin(int client)
{
    char id[8];
    NCS_Cookie_Get(client, "skin_used", id, sizeof(id));
    g_UsersUsedSkin[client] = StringToInt(id);
    CheckUserHaveSkin(client);
}

void SetUsedSkin(int client, const char[] id)
{
    NCS_Cookie_Set(client, "skin_used", id);
    g_UsersUsedSkin[client] = StringToInt(id);
}