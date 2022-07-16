void StartCheck(int client)
{
	CreateTimer(30.0, Timer_Check, client, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

public Action Timer_Check(Handle timer, int client)
{
	if (g_Accounts[client].Valid())
		return Plugin_Stop;
	
	if (!IsValidClient(client))
		return Plugin_Stop;

	char steamid[32];
	if (!GetClientAuthId(client, AuthId_SteamID64, steamid, sizeof(steamid), false))
		return Plugin_Stop;

	ReqAccountUID(steamid, true);
	return Plugin_Continue;
}