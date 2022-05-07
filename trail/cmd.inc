void InitCmd()
{
	RegConsoleCmd("sm_trail", Command_Trail, "[NCS] 更换足迹");
	RegConsoleCmd("sm_trails", Command_Trail, "[NCS] 更换足迹");
}

public Action Command_Trail(int client, int args)
{
	DisplayUserTrails(client);
	return Plugin_Handled;
}
