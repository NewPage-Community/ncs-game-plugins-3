void RegCmd()
{
	RegServerCmd("ncs_server_restart", Command_Restart);
	RegServerCmd("ncs_server_reloadmap", Command_ReloadMap);
	RegServerCmd("ncs_server_reload", Command_Reload);
}

public Action Command_Restart(int args)
{
	NCS_Chat(CLIENT_ALL, _, "服务器将进行重启更新!");
	PrintCenterTextAll("服务器将进行重启更新!");
	CreateTimer(1.0, Timer_Restart, 0);
}

public Action Command_ReloadMap(int args)
{
	NCS_Chat(CLIENT_ALL, _, "服务器将在 {green}10秒 {default}后进行热更新，执行地图重载!");
	PrintCenterTextAll("服务器将在 10秒 后进行热更新，执行地图重载!");
	CreateTimer(10.0, Timer_ReloadMap, 0);

	return Plugin_Handled;
}

public Action Timer_Restart(Handle timer, int time)
{
	if(time < 10)
	{
		NCS_Chat(CLIENT_ALL, _, "服务器将在 {green}%d秒{default} 后重启!", 10 - time);
		PrintCenterTextAll("服务器将在 %d秒 后重启!", 10 - time);
	}
	else if (time == 11)
	{
		for(int i = 1; i <= MaxClients; i++)
			if (IsClientInGame(i))
				ClientCommand(i, "retry");
	}
	else if (time == 12)
	{
		ServerCommand("quit");
		return Plugin_Stop;
	}
	CreateTimer(1.0, Timer_Restart, time + 1);
	return Plugin_Stop;
}

public Action Timer_ReloadMap(Handle timer, int time)
{
	char map[PLATFORM_MAX_PATH];
	GetCurrentMap(map, PLATFORM_MAX_PATH);
	ServerCommand("changelevel %s", map);
	return Plugin_Stop;
}

public Action Command_Reload(int args)
{
	ReqServerInfo();
	PrintToServer("[NCS] Server - Reloading...");
	return Plugin_Handled;
}