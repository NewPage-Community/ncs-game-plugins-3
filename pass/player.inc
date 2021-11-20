static Handle playerTimer[MAXPLAYERS+1];

void InitPlayer(int client, const char[] uid)
{
    playerPass[client].Init(client, uid);

    if (playerTimer[client] != INVALID_HANDLE)
        delete playerTimer[client];
    playerTimer[client] = CreateTimer(1.0, Timer_AddPoint, client, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

void PlayerDisconnect(int client)
{
    delete playerTimer[client];
    Timer_UpdatePoint(INVALID_HANDLE, client);
    playerPass[client].Clear();
}

public Action Timer_AddPoint(Handle timer, int client)
{
    if (!playerPass[client].IsValid())
        return Plugin_Stop;

    if (IsClientInGame(client))
        playerPass[client].AddPoint(1);

    return Plugin_Continue;
}