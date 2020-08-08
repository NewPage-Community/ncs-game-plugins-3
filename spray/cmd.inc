void InitCmd()
{
    RegConsoleCmd("sm_spray", Command_SprayMenu, "[NCS] 更换光环");
    RegConsoleCmd("sm_sprays", Command_SprayMenu, "[NCS] 更换光环");
    RegConsoleCmd("spray", Command_Spray);
    RegConsoleCmd("sprays", Command_Spray);
}

public Action Command_SprayMenu(int client, int args)
{
    DisplayUserSprays(client);
    return Plugin_Handled;
}

public Action Command_Spray(int client, int args)
{
    CreateSprays(client);
    return Plugin_Handled;
}
