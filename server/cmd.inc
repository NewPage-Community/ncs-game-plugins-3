#include <ncs/chat>

void RegCmd()
{
	RegServerCmd("ncs_server_restart", Command_Restart);
	RegServerCmd("ncs_server_reloadmap", Command_ReloadMap);
	RegServerCmd("ncs_server_reload", Command_Reload);
}

public Action Command_Restart(int args)
{
	NCS_Chat(CLIENT_ALL, "[服务器提示]", "{blue}服务器将进行重启更新!");
	PrintCenterTextAll("服务器将进行重启更新!");
	CreateTimer(1.0, Timer_Restart, 0);
}

public Action Command_ReloadMap(int args)
{
	NCS_Chat(CLIENT_ALL, "[服务器提示]", "{blue}服务器将在5秒后进行热更新，执行地图重载!");
	PrintCenterTextAll("服务器将在3秒后进行热更新，执行地图重载!");
	CreateTimer(5.0, Timer_ReloadMap, 0);

	return Plugin_Handled;
}

public Action Timer_Restart(Handle timer, int time)
{
	if(time < 10)
	{
		NCS_Chat(CLIENT_ALL, "[服务器提示]", "服务器将在 \x04%d\x01 秒后重启!");
		PrintCenterTextAll("服务器将在 %ds 后重启!", 10 - time);
		CreateTimer(1.0, Timer_Restart, time + 1);
		return Plugin_Stop;
	}

	for(int i = 1; i <= MaxClients; i++)
		if (IsClientInGame(i))
				ClientCommand(i, "retry");

	ServerCommand("quit");

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