void PlayerInit(int client)
{
    char name[MAX_NAME_LENGTH];
    NCS_Account_GetName(client, name, sizeof(name));
    NCS_Chat(_, _, "{blue}欢迎 {green}%s {red}(%d级VIP) 进入服务器", name, g_VIPs[client].Level());

    // Auto Sign
    CreateTimer(5.0, Timer_AutoSign, client, TIMER_FLAG_NO_MAPCHANGE);
}

public Action Timer_AutoSign(Handle timer, int client)
{
    if (g_VIPs[client].IsValid())
    {
        FakeClientCommandEx(client, "sm_sign");
    }
    return Plugin_Stop;
}