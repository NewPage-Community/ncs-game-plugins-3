void GetUsedSkin(int client)
{
	static char id[8];
	NCS_Cookie_Get(client, "skin_used", id, sizeof(id));
	g_UsersUsedSkin[client] = StringToInt(id);

	if (!CheckUserInSkinGroup(client, g_UsersUsedSkin[client]) && !IsSkinAdmin(client))
		CheckUserHaveSkin(client);
#if defined GAME_INSURGENCY
	else
		SetPlayerModelExt(client, g_UsersUsedSkin[client]);
#endif
}

void SetUsedSkin(int client, int id)
{
	static char buffer[16];
	IntToString(id, buffer, sizeof(buffer));
	NCS_Cookie_Set(client, "skin_used", buffer);
	g_UsersUsedSkin[client] = id;
}

stock int GetUserSkinBody(int client, int id)
{
	static char buffer[16];
	Format(buffer, sizeof(buffer), "skin_body:%d", id);
	if (!NCS_Cookie_Get(client, buffer, buffer, sizeof(buffer)))
		return -1;
	return StringToInt(buffer);
}

stock void SetUserSkinBody(int client, int id, int value)
{
	static char buffer[16], val[16];
	Format(buffer, sizeof(buffer), "skin_body:%d", id);
	IntToString(value, val, sizeof(val));
	NCS_Cookie_Set(client, buffer, val);
}

stock int GetUserSkinSkin(int client, int id)
{
	static char buffer[16];
	Format(buffer, sizeof(buffer), "skin_skin:%d", id);
	if (!NCS_Cookie_Get(client, buffer, buffer, sizeof(buffer)))
		return -1;
	return StringToInt(buffer);
}

stock void SetUserSkinSkin(int client, int id, int value)
{
	static char buffer[16], val[16];
	Format(buffer, sizeof(buffer), "skin_skin:%d", id);
	IntToString(value, val, sizeof(val));
	NCS_Cookie_Set(client, buffer, val);
}

stock int GetUserSkinSound(int client)
{
	static char buffer[16];
	if (!NCS_Cookie_Get(client, "skin_sound", buffer, sizeof(buffer)))
	{
		// Merge old cookie
		if (NCS_Cookie_Get(client, "skin_radio_off", buffer, sizeof(buffer)))
		{
			if (StringToInt(buffer) == 1)
			{
				SetUserSkinSound(client, 0);
				return 0;
			}
		}
		else
			return -1;
	}
	return StringToInt(buffer);
}

stock void SetUserSkinSound(int client, int value)
{
	static char val[16];
	IntToString(value, val, sizeof(val));
	NCS_Cookie_Set(client, "skin_sound", val);
}

stock int GetUserSkinArm(int client)
{
	static char buffer[16];
	if (!NCS_Cookie_Get(client, "skin_arm", buffer, sizeof(buffer)))
	{
		// Merge old cookie
		if (NCS_Cookie_Get(client, "skin_arms_off", buffer, sizeof(buffer)))
		{
			if (StringToInt(buffer) == 1)
			{
				SetUserSkinSound(client, 0);
				return 0;
			}
		}
		else
			return -1;
	}
	return StringToInt(buffer);
}

stock void SetUserSkinArm(int client, int value)
{
	static char val[16];
	IntToString(value, val, sizeof(val));
	NCS_Cookie_Set(client, "skin_arm", val);
}
