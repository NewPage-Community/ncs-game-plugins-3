bool g_PlayerRadioOff[MAXPLAYERS + 1];

void SkinRadioOffGetCookie(int client)
{
	static char buffer[8];
	if (NCS_Cookie_Get(client, "skin_radio_off", buffer, sizeof(buffer)))
		g_PlayerRadioOff[client] = StringToInt(buffer) == 1;
	else
		g_PlayerRadioOff[client] = false;
}

void SkinRadioOffSet(int client, bool off)
{
	static char buffer[8];
	IntToString(off, buffer, sizeof(buffer));
	g_PlayerRadioOff[client] = off;
	NCS_Cookie_Set(client, "skin_radio_off", buffer);
}

bool SkinRadioOff(int client)
{
	return g_PlayerRadioOff[client];
}