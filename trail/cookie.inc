void GetUsedTrail(int client)
{
    char id[8];
    NCS_Cookie_Get(client, "trail_used", id, sizeof(id));
    g_UsersUsedTrail[client] = StringToInt(id);
    CheckUserHaveTrail(client);
}