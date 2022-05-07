void SetModel(int client)
{
	if (!IsClientInGame(client) || !IsPlayerAlive(client) || IsFakeClient(client))
		return;

	if (isZombieReloadedLoaded)
		if (ZR_IsClientZombie(client))
			return;
	
	// Team limit
	if (cv_skin_only_team.IntValue > 0 && cv_skin_only_team.IntValue != GetClientTeam(client))
		return;

	int skinID = g_UsersUsedSkin[client];
	if (skinID > 0)
	{
		Skin s;
		if (!g_Skins.Get(skinID, s))
			return;

		if (!IsModelPrecached(s.skinPath))
			return;

		SetEntityModel(client, s.skinPath);
		SetEntityRenderColor(client, 255, 255, 255, 255);

		if (IsModelPrecached(s.armPath) && s.armPath[0] != '\0' && !SkinArmsOff(client))
			SetEntPropString(client, Prop_Send, "m_szArmsModel", s.armPath);
	}

	SkinSoundOnPlayerSpawn(client);
}

void Model_PlayerSpawn(int client)
{
	CreateTimer(1.0, ModelCheck, client, TIMER_FLAG_NO_MAPCHANGE);
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

		char sModelPath[PLATFORM_MAX_PATH];
		GetEntPropString(client, Prop_Data, "m_ModelName", sModelPath, sizeof(sModelPath));
		if (StrContains(sModelPath, s.skinPath, true) == -1)
			RequestFrame(SetModel, client);
	}

	return Plugin_Continue;
}
