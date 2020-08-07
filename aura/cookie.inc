#define COOKIE_AURA_KEY "aura_used"

void GetUsedAura(int client)
{
    char id[8];
    NCS_Cookie_Get(client, COOKIE_AURA_KEY, id, sizeof(id));
    g_UsersUsedAura[client] = StringToInt(id);
    CheckUserHaveAura(client);
}

void SetUsedAura(int client, const char[] id)
{
    NCS_Cookie_Set(client, COOKIE_AURA_KEY, id);
    g_UsersUsedAura[client] = StringToInt(id);
    SetClientAura(client);
}