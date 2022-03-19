#define DEFAULT_SOUND_VOL 70

int g_PlayerRadioVolume[MAXPLAYERS + 1][2];

void SkinRadioVolumeGetCookie(int client)
{
    static char buffer[8];
    if (NCS_Cookie_Get(client, "skin_radio_volume_myself", buffer, sizeof(buffer)))
        g_PlayerRadioVolume[client][0] = StringToInt(buffer);
    else
        g_PlayerRadioVolume[client][0] = DEFAULT_SOUND_VOL;
    if (NCS_Cookie_Get(client, "skin_radio_volume_other", buffer, sizeof(buffer)))
        g_PlayerRadioVolume[client][1] = StringToInt(buffer);
    else
        g_PlayerRadioVolume[client][1] = DEFAULT_SOUND_VOL;
}

void SkinRadioVolumeSet(int client, int volume, int other)
{
    static char buffer[8];
    IntToString(volume, buffer, sizeof(buffer));
    g_PlayerRadioVolume[client][other] = volume;
    if (other) NCS_Cookie_Set(client, "skin_radio_volume_other", buffer);
    else NCS_Cookie_Set(client, "skin_radio_volume_myself", buffer);
}

int SkinRadioVolumeGet(int client, int other)
{
    return g_PlayerRadioVolume[client][client == other ? 0 : 1];
}