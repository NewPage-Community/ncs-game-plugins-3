static Handle playerRadioTimer[MAXPLAYERS + 1];

bool SkinPlaySound(int client, const char[] cmd, bool radius = false)
{
    int skinID = g_UsersUsedSkin[client];
    if (!skinID) return false;

    Skin skin;
    if (!g_Skins.Get(skinID, skin)) return false;

    static char sample[PLATFORM_MAX_PATH];
    if (!skin.GetSound(cmd, sample, sizeof(sample))) return false;
    EmitSoundToAll(sample, radius ? client : SOUND_FROM_PLAYER);
    NCS_Chat(CLIENT_ALL, "[皮肤音效]", "{olive}%N{default} ({lightblue}%s{default}) 使用了音效 {olive}%s", client, skin.name, cmd);
    return true;
}

void SkinSoundOnPlayerSpawn(int client)
{
    SkinPlaySound(client, "spawn", true);
}

void SkinSoundOnPlayerDeath(int client)
{
    SkinPlaySound(client, "death", true);
}

bool SkinSoundOnPlayerRadio(int client, const char[] cmd)
{
    if (playerRadioTimer[client]) return true;
    bool success = SkinPlaySound(client, cmd);
    // Root skip timer
    if (!(GetUserFlagBits(client) & ADMFLAG_ROOT) && success)
        CreateTimer(30.0, SkinTimer, client, TIMER_FLAG_NO_MAPCHANGE);
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
