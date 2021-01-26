void SetModel(int client)
{
    if (!IsClientInGame(client) || !IsPlayerAlive(client) || IsFakeClient(client))
        return;

    int skinID = g_UsersUsedSkin[client];
    if (skinID > 0)
    {
        Skin s;
        if (!g_Skins.Get(skinID, s))
            return;

        // Support SDK2013
        if (IsSDK2013Mod())
            ClientCommand(client, "cl_playermodel %s", s.skinPath);
        else
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
    int skinID = g_UsersUsedSkin[client];
    if (skinID > 0)
    {
        if (!g_Skins.Get(skinID, s))
            return Plugin_Continue;

        // Support SDK2013
        if (IsSDK2013Mod())
        {
            DataPack dp = new DataPack();
            dp.WriteString(s.skinPath);
            QueryClientConVar(client, "cl_playermodel", ModelCheckCallback, dp);
        }
        else
        {
            char sModelPath[PLATFORM_MAX_PATH];
            GetEntPropString(client, Prop_Data, "m_ModelName", sModelPath, sizeof(sModelPath));
            if (StrContains(sModelPath, s.skinPath, true) == -1)
                RequestFrame(SetModel, client);
        }
    }
    else
        if (IsSDK2013Mod())
            ClientCommand(client, "cl_playermodel models/human");

    return Plugin_Continue;
}

void Broadcast_DeathSound(int client)
{
    if(!IsClientInGame(client) || IsFakeClient(client))
        return;

    int skinID = g_UsersUsedSkin[client];
    if (skinID <= 0)
        return;

    Skin s;
    if (!g_Skins.Get(skinID, s) || s.deathSound[0] == '\0')
        return;

    float fPos[3], fAgl[3];
    GetClientEyePosition(client, fPos);
    GetClientEyeAngles(client, fAgl);
    fPos[2] -= 3.0;
    int speaker = SpawnSpeakerEntity(fPos, fAgl, client, 10.0);

    EmitSoundToAll(s.deathSound, speaker, _, _, _, 0.8, _, speaker, fPos, fAgl, true);
}

int SpawnSpeakerEntity(float fPos[3], float fAgl[3], int source, float removeDelay = 0.1)
{
    int speaker = CreateEntityByName("info_target");

    if(speaker == -1)
        return -1;

    DispatchSpawn(speaker);

    TeleportEntity(speaker, fPos, fAgl, NULL_VECTOR);

    SetVariantString("!activator");
    AcceptEntityInput(speaker, "SetParent", source, speaker, 0);

    if(removeDelay > 0.0)
    {
        char input[128];
        FormatEx(input, 128, "OnUser4 !self:Kill::%.2f:1", removeDelay);
        SetVariantString(input);
        AcceptEntityInput(speaker, "AddOutput");
        AcceptEntityInput(speaker, "FireUser4");
    }

    return speaker;
}

void ModelCheckCallback(QueryCookie cookie, int client, ConVarQueryResult result, const char[] cvarName, const char[] cvarValue, DataPack dp)
{
    if (result == ConVarQuery_Okay)
    {
        char sModelPath[PLATFORM_MAX_PATH];
        dp.Reset();
        dp.ReadString(sModelPath, sizeof(sModelPath));
        if (StrContains(cvarValue, sModelPath, true) == -1)
            RequestFrame(SetModel, client);
    }
    delete dp;
}