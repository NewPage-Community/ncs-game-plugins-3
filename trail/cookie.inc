void GetUsedTrail(int client)
{
    char id[8];
    NCS_Cookie_Get(client, "trail_used", id, sizeof(id));
    g_UsersUsedTrail[client] = StringToInt(id);
    CheckUserHaveTrail(client);
}

void SetUsedTrail(int client, const char[] id)
{
    NCS_Cookie_Set(client, "trail_used", id);
    g_UsersUsedTrail[client] = StringToInt(id);
    CreateTrail(client);
}