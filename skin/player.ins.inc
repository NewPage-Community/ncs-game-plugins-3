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

void SetPlayerModelExt(int client, int skinID)
{
    if (skinID <= 0)
        return;

    Skin skin;
    if (!g_Skins.Get(skinID, skin))
        return;
    
    int body = GetUserSkinBody(client, skinID);
    SetPlayerModel(client, skin.skinPath, body, true);
}

void ClearPlayerModel(int client)
{
    SetPlayerModel(client, "", 0);
}
