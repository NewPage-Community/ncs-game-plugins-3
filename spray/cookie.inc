#define COOKIE_SPRAY_KEY "spray_used"

void GetUsedSpray(int client)
{
    char id[8];
    NCS_Cookie_Get(client, COOKIE_SPRAY_KEY, id, sizeof(id));
    g_UsersUsedSpray[client] = StringToInt(id);
    CheckUserHaveSpray(client);
}

void SetUsedSpray(int client, const char[] id)
{
    NCS_Cookie_Set(client, COOKIE_SPRAY_KEY, id);
    g_UsersUsedSpray[client] = StringToInt(id);
}