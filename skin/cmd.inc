void InitCmd()
{
    RegCmd();
}

void RegCmd()
{
    RegConsoleCmd("sm_skin", Command_Skin, "[NCS] 更换皮肤");
    RegConsoleCmd("sm_skins", Command_Skin, "[NCS] 更换皮肤");
    RegConsoleCmd("sm_model", Command_Skin, "[NCS] 更换皮肤");
    RegConsoleCmd("sm_models", Command_Skin, "[NCS] 更换皮肤");
}

public Action Command_Skin(int client, int args)
{
    DisplayUserSkins(client);
    return Plugin_Handled;
}
