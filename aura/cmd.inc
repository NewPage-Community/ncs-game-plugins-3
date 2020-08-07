void InitCmd()
{
    RegConsoleCmd("sm_aura", Command_Aura, "[NCS] 更换光环");
    RegConsoleCmd("sm_auras", Command_Aura, "[NCS] 更换光环");
}

public Action Command_Aura(int client, int args)
{
    DisplayUserAuras(client);
    return Plugin_Handled;
}
