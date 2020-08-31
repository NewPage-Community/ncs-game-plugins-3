void InitCmd()
{
    RegServerCmd("ncs_convar_update", Command_Update, "[NCS] 重载控制台变量");
}

public Action Command_Update(int args)
{
    UpdateCVars();
    return Plugin_Handled;
}