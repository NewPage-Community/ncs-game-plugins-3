void InitCmd()
{
    RegConsoleCmd("sm_spray", Command_SprayMenu, "[NCS] 更换光环");
    RegConsoleCmd("sm_sprays", Command_SprayMenu, "[NCS] 更换光环");
}

public Action Command_SprayMenu(int client, int args)
{
    DisplayUserSprays(client);
    return Plugin_Handled;
}
