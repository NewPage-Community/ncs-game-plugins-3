void SetModel(int client)
{
    if (!IsClientInGame(client) || !IsPlayerAlive(client) || IsFakeClient(client))
        return;
    
    // For Insurgency PVE
    if (GetClientTeam(client) != 2)
        return;

    int skinID = g_UsersUsedSkin[client];
    if (skinID > 0)
    {
        Skin s;
        if (!g_Skins.Get(skinID, s))
            return;
        SetEntityModel(client, s.skinPath);
    }
}

void Model_PlayerSpawn(int client)
{
    CreateTimer(1.0, ModelCheck, client, TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
}

public Action ModelCheck(Handle timer, int client)
{
    if (!IsClientInGame(client) || !IsPlayerAlive(client) || IsFakeClient(client))
        return Plugin_Stop;

    Skin s;
    char sModelPath[PLATFORM_MAX_PATH];
    int skinID = g_UsersUsedSkin[client];
    if (!g_Skins.Get(skinID, s))
        return Plugin_Stop;

    GetEntPropString(client, Prop_Data, "m_ModelName", sModelPath, sizeof(sModelPath));
    if (StrContains(sModelPath, s.skinPath, true) == -1)
    {
        RequestFrame(SetModel, client);
    }

    return Plugin_Continue;
}
