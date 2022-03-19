void InitCmd()
{
    RegCmd();
}

void RegCmd()
{
    RegConsoleCmd("sm_sign", Command_Sign, "[NCS] 用户签到");
    RegConsoleCmd("sm_qd", Command_Sign, "[NCS] 用户签到");
}

public Action Command_Sign(int client, int args)
{
    Sign(client);
    return Plugin_Handled;
}
