static Handle playerRadioTimer[MAXPLAYERS + 1];

bool SkinPlaySound(int client, const char[] cmd, bool radius = false, bool myself = false, bool broadcast = false)
{
    if (IsFakeClient(client)) return false;

    int skinID = g_UsersUsedSkin[client];
    if (!skinID) return false;

    Skin skin;
    if (!g_Skins.Get(skinID, skin)) return false;

    static char sample[PLATFORM_MAX_PATH];
    if (!skin.GetSound(cmd, sample)) return false;
    if (myself)
    {
        float volume = SkinRadioVolumeGet(client, client) / 100.0;
        if (volume) EmitSoundToClient(client, sample, radius ? client : SOUND_FROM_PLAYER, _, _, _, volume);
    }
    else
    {
        for (int i = 1; i <= MaxClients; i++)
        {
            if (!IsClientConnected(i)) continue;
            if (IsFakeClient(i)) continue;
            PlayerRadioBlock blocker;
            blocker = g_PlayerRadioBlock[i];
            if (blocker.Mode == 1 && i != client) continue;
            if (blocker.Mode == 2 && blocker.BlackList.FindValue(skin.id) != -1) continue;
            if (blocker.Mode == 3 && blocker.WhiteList.FindValue(skin.id) == -1) continue;
            float volume = SkinRadioVolumeGet(i, client) / 100.0;
            if (radius) volume = 1.0;
            if (volume) EmitSoundToClient(client, sample, radius ? client : SOUND_FROM_PLAYER, _, _, _, volume);
        }
    }

    if (!broadcast) return true;

    char cmdt[16];
#if defined GAME_INSURGENCY
    TranslateCmd(cmd, cmdt, sizeof(cmdt));
#else
    strcopy(cmdt, sizeof(cmdt), cmd);
#endif
    if (!radius)
        NCS_Chat(CLIENT_ALL, "[皮肤音效]", "{olive}%N{default} ({lightblue}%s{default}) 使用了音效 {olive}%s", client, skin.name, cmdt);
    return true;
}

void SkinSoundOnPlayerSpawn(int client)
{
    if (!IsPlayerAlive(client)) return;
    SkinPlaySound(client, "spawn", false, true);
}

stock void SkinSoundOnPlayerDeath(int client)
{
    SkinPlaySound(client, "death", true);
}

bool SkinSoundOnPlayerRadio(int client, const char[] cmd)
{
    if (SkinRadioOff(client)) return false;
    if (playerRadioTimer[client]) return true;
    bool success = SkinPlaySound(client, cmd, false, false, true);
    if (success && !(GetUserFlagBits(client) & ADMFLAG_ROOT))
        playerRadioTimer[client] = CreateTimer(10.0, SkinTimer, client, TIMER_FLAG_NO_MAPCHANGE);
    return success;
}

void SkinSoundOnClientConnected(int client)
{
    playerRadioTimer[client] = INVALID_HANDLE;
}

public Action SkinTimer(Handle timer, int client)
{
    playerRadioTimer[client] = INVALID_HANDLE;
    return Plugin_Stop;
}

void SkinSoundOnRoundEnd(int winner)
{
    for (int client = 1; client <= MaxClients; client++)
    {
        if (!IsClientInGame(client)) continue;
        if (IsFakeClient(client)) continue;
        if (GetClientTeam(client) == winner)
            SkinPlaySound(client, "win", false, true);
        else
            SkinPlaySound(client, "lose", false, true);
    }
}

stock bool SkinGetSound(int client, const char[] cmd, char sample[PLATFORM_MAX_PATH])
{
    int skinID = g_UsersUsedSkin[client];
    if (!skinID) return false;
    Skin skin;
    if (!g_Skins.Get(skinID, skin)) return false;
    return skin.GetSound(cmd, sample);
}