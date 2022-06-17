public Action InsSoundHook(int clients[MAXPLAYERS], int &numClients, char sample[PLATFORM_MAX_PATH],
	  int &entity, int &channel, float &volume, int &level, int &pitch, int &flags,
	  char soundEntry[PLATFORM_MAX_PATH], int &seed)
{
	static int overrideClients[MAXPLAYERS], overrideNumClients;
	static char buffer[PLATFORM_MAX_PATH], overrideSample[PLATFORM_MAX_PATH];
	bool overrideSound = false;
	if (entity < 1 || entity >= MaxClients) return Plugin_Continue;
	if (!strncmp(sample, "newpage/models", 14)) return Plugin_Continue;
	if (StrContains(sample, "pl_deathshout_") != -1) return Plugin_Stop;
	if (!strncmp(sample, "player/voice/responses", 22))
	{
		if (IsFakeClient(entity)) return Plugin_Continue;
		strcopy(buffer, PLATFORM_MAX_PATH, sample[22]);
		if (StrContains(buffer, "rpg") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "rpg", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "molotov_incendiary_detonated_close") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "hurt", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "molotov_incendiary_pain_light") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "hurt", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "wounded") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "hurt", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "suppressed/suppressed") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "suppress", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "reloading") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "reload", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "smoke") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "grenade", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "flash") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "grenade", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "frag") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "grenade", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "incendiarythrow") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "grenade", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
		else if (StrContains(buffer, "molotovthrow") != -1)
		{
			overrideSound = SkinSoundGetPlay(entity, "grenade", overrideSample, clients, numClients, overrideClients, overrideNumClients)
		}
	}
	if (overrideSound)
	{
		EmitSound(overrideClients, overrideNumClients, overrideSample, entity, channel, level, flags, volume, pitch);
		return Plugin_Changed;
	}
	return Plugin_Continue;
}

bool SkinSoundGetPlay(int client, const char[] cmd, char overrideSample[PLATFORM_MAX_PATH], int clients[MAXPLAYERS], int &numClients, int overrideClients[MAXPLAYERS], int &overrideNumClients)
{
	int skinID = g_UsersUsedSkin[client];
	if (!skinID) return false;
	Skin skin;
	if (!g_Skins.Get(skinID, skin)) return false;
	if (!skin.GetSound(cmd, overrideSample)) return false;
	numClients = 0;
	overrideNumClients = 0;
	for (int i = 1; i <= MAXPLAYERS; i++)
	{
		if (!IsClientConnected(i)) continue;
		if (IsFakeClient(i) || !IsClientInGame(i)) continue;
		if (SkinSoundIsBlock(client, i, skin))
			clients[numClients++] = i;
		else
			overrideClients[overrideNumClients++] = i;
	}
	return true;
}
