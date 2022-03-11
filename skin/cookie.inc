void GetUsedSkin(int client)
{
    static char id[8];
    NCS_Cookie_Get(client, "skin_used", id, sizeof(id));
    g_UsersUsedSkin[client] = StringToInt(id);
    CheckUserHaveSkin(client);
}

void SetUsedSkin(int client, int id)
{
    static char buffer[16];
    IntToString(id, buffer, sizeof(buffer));
    NCS_Cookie_Set(client, "skin_used", buffer);
    g_UsersUsedSkin[client] = id;
}

stock int GetUserSkinBody(int client, int id)
{
    static char buffer[16];
    Format(buffer, sizeof(buffer), "skin_body:%d", id);
    NCS_Cookie_Get(client, buffer, buffer, sizeof(buffer));
    return StringToInt(buffer);
}

stock void SetUserSkinBody(int client, int id, int value)
{
    static char buffer[16], val[16];
    Format(buffer, sizeof(buffer), "skin_body:%d", id);
    IntToString(value, val, sizeof(val));
    NCS_Cookie_Set(client, buffer, val);
}