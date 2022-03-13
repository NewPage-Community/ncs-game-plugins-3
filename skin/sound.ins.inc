public Action InsSoundHook(int clients[MAXPLAYERS], int &numClients, char sample[PLATFORM_MAX_PATH],
	  int &entity, int &channel, float &volume, int &level, int &pitch, int &flags,
	  char soundEntry[PLATFORM_MAX_PATH], int &seed)
{
    static char buffer[PLATFORM_MAX_PATH];
    if (entity < 1 || entity >= MaxClients) return Plugin_Continue;
    if (!strncmp(sample, "newpage/models", 14)) return Plugin_Continue;
    if (StrContains(sample, "pl_deathshout_") != -1)
    {
        if (IsFakeClient(entity)) return Plugin_Stop;
        if (SkinGetSound(entity, "death", sample)) return Plugin_Changed;
    }
    if (!strncmp(sample, "player/voice/responses", 22))
    {
        if (IsFakeClient(entity)) return Plugin_Continue;
        strcopy(buffer, PLATFORM_MAX_PATH, sample[22]);
        if (StrContains(buffer, "rpg") != -1)
        {
            if (SkinGetSound(entity, "rpg", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "molotov_incendiary_detonated_close") != -1)
        {
            if (SkinGetSound(entity, "hurt", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "molotov_incendiary_pain_light") != -1)
        {
            if (SkinGetSound(entity, "hurt", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "wounded") != -1)
        {
            if (SkinGetSound(entity, "hurt", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "suppressed/suppressed") != -1)
        {
            if (SkinGetSound(entity, "suppress", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "reloading") != -1)
        {
            if (SkinGetSound(entity, "reload", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "smoke") != -1)
        {
            if (SkinGetSound(entity, "grenade", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "flash") != -1)
        {
            if (SkinGetSound(entity, "grenade", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "frag") != -1)
        {
            if (SkinGetSound(entity, "grenade", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "incendiarythrow") != -1)
        {
            if (SkinGetSound(entity, "grenade", sample)) return Plugin_Changed;
        }
        if (StrContains(buffer, "molotovthrow") != -1)
        {
            if (SkinGetSound(entity, "grenade", sample)) return Plugin_Changed;
        }
    }
    return Plugin_Continue;
}