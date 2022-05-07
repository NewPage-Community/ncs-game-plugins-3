bool g_PlayerArmsOff[MAXPLAYERS + 1];

void SkinArmsOffGetCookie(int client)
{
	static char buffer[8];
	if (NCS_Cookie_Get(client, "skin_arms_off", buffer, sizeof(buffer)))
		g_PlayerArmsOff[client] = StringToInt(buffer) == 1;
	else
		g_PlayerArmsOff[client] = false;
}

void SkinArmsOffSet(int client, bool off)
{
	static char buffer[8];
	IntToString(off, buffer, sizeof(buffer));
	g_PlayerArmsOff[client] = off;
	NCS_Cookie_Set(client, "skin_arms_off", buffer);
}

bool SkinArmsOff(int client)
{
	return g_PlayerArmsOff[client];
}