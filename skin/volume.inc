#define SKIN_RADIO_DEFAULT_VOL 70
#define SKIN_RADIO_VOL_MYSELF 0
#define SKIN_RADIO_VOL_OTHER 1

int g_PlayerRadioVolume[MAXPLAYERS + 1][2];

void SkinRadioVolumeGetCookie(int client)
{
	static char buffer[8];
	if (NCS_Cookie_Get(client, "skin_radio_volume_myself", buffer, sizeof(buffer)))
		g_PlayerRadioVolume[client][0] = StringToInt(buffer);
	else
		g_PlayerRadioVolume[client][0] = SKIN_RADIO_DEFAULT_VOL;
	if (NCS_Cookie_Get(client, "skin_radio_volume_other", buffer, sizeof(buffer)))
		g_PlayerRadioVolume[client][1] = StringToInt(buffer);
	else
		g_PlayerRadioVolume[client][1] = SKIN_RADIO_DEFAULT_VOL;
}

void SkinRadioVolumeSet(int client, int type, int volume)
{
	static char buffer[8];
	IntToString(volume, buffer, sizeof(buffer));
	g_PlayerRadioVolume[client][type] = volume;
	if (type == SKIN_RADIO_VOL_MYSELF)
		NCS_Cookie_Set(client, "skin_radio_volume_myself", buffer);
	else // SKIN_RADIO_VOL_OTHER
		NCS_Cookie_Set(client, "skin_radio_volume_other", buffer);
}

int SkinRadioVolumeGet(int client, int type)
{
	return g_PlayerRadioVolume[client][type];
}