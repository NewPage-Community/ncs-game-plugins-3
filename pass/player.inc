static Handle playerTimer[MAXPLAYERS+1];

void InitPlayer(int client, const char[] uid)
{
    player[client].Init(client, uid);

    if (playerTimer[client] != INVALID_HANDLE)
        delete playerTimer[client];
    playerTimer[client] = CreateTimer(1.0, Timer_AddPoint, client, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

void PlayerDisconnect(int client)
{
    delete playerTimer[client];
    Timer_UpdatePoint(INVALID_HANDLE, client);
    player[client].Clear();
}

public Action Timer_AddPoint(Handle timer, int client)
{
    if (!player[client].IsValid())
        return Plugin_Stop;

    if (IsClientInGame(client))
        player[client].AddPoint(1);

    return Plugin_Continue;
}