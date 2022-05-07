static int playerAura[MAXPLAYERS+1];
static int g_iPreviewEnt[2048];
static Handle g_tPreview[MAXPLAYERS+1];

void SetClientAura(int client, int id = 0, bool preview = false)
{
	if (!IsClientInGame(client) || !IsPlayerAlive(client) || IsFakeClient(client))
		return;

	if (isZombieReloadedLoaded)
		if (ZR_IsClientZombie(client))
			return;

	// Kill preview timer
	Handle timer = g_tPreview[client];
	g_tPreview[client] = INVALID_HANDLE;
	if(timer != INVALID_HANDLE)
		KillTimer(timer);

	if (playerAura[client] != INVALID_ENT_REFERENCE)
		RemoveClientAura(client);

	if (!preview)
	{
		id = g_UsersUsedAura[client];
		if (id == 0)
			return;
	}

	Aura s;
	if (!auras.Get(id, s))
		return;

	int entity = CreateAura(client, s.particleName);

	playerAura[client] = EntIndexToEntRef(entity);

	if (preview)
	{
		g_iPreviewEnt[entity] = client;
		SDKHook(entity, SDKHook_SetTransmit, Hook_SetTransmit_Preview);
		g_tPreview[client] = CreateTimer(30.0, Timer_ClearPre, client);
		NCS_Chat(client, _, "预览光环 {lime}%s{default} 30秒，输入 {green}!tp{default} 查看效果", s.name);
	}
}

int CreateAura(int client, const char[] name)
{
	float clientOrigin[3];
	GetClientAbsOrigin(client, clientOrigin);

	int entity = CreateEntityByName("info_particle_system");
	DispatchKeyValue(entity, "start_active", "1");
	DispatchKeyValue(entity, "effect_name", name);
	DispatchSpawn(entity);
	TeleportEntity(entity, clientOrigin, NULL_VECTOR, NULL_VECTOR);
	SetVariantString("!activator");
	AcceptEntityInput(entity, "SetParent", client, entity, 0);
	ActivateEntity(entity);
	return entity;
}

void RemoveClientAura(int client)
{
	if(playerAura[client] != INVALID_ENT_REFERENCE)
	{
		int entity = EntRefToEntIndex(playerAura[client]);
		if(entity > 0 && IsValidEdict(entity))
		{
			AcceptEntityInput(entity, "Kill");
		}
	}

	playerAura[client] = INVALID_ENT_REFERENCE;
}

public Action Timer_ClearPre(Handle timer, int client)
{
	g_tPreview[client] = INVALID_HANDLE;
	RemoveClientAura(client);
	SetClientAura(client);
	return Plugin_Stop;
}

public Action Hook_SetTransmit_Preview(int ent, int client)
{
	return (g_iPreviewEnt[ent] == client) ? Plugin_Continue : Plugin_Handled;
}

public void ZR_OnClientInfected(int client, int attacker, bool motherInfect, bool respawnOverride, bool respawn)
{
	if (IsValidClient(client))
		RemoveClientAura(client);
}