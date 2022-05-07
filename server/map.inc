ConVar changeMapNotify;
int lastRoundPlayer;

void InitMap()
{
	changeMapNotify = CreateConVar("ncs_server_change_map_notify", "1", "Change map notify");
	HookEvent("round_end", OnRoundEnd);
}

void MapNotify()
{
	if (changeMapNotify.BoolValue && ServerID > 0 && lastRoundPlayer > 5)
	{
		char map[PLATFORM_MAX_PATH];
		GetCurrentMap(map, sizeof(map));
		ReqMapNotify(ServerID, map);
	}
}

public void OnRoundEnd(Event event, const char[] name1, bool dontBroadcast)
{
	CountPlayer();
}

void CountPlayer()
{
	lastRoundPlayer = 0;
	for (int i = 1; i <= MaxClients; i++)
	{
		if (!IsClientInGame(i)) continue;
		if (IsFakeClient(i)) continue;
		lastRoundPlayer++;
	}
}