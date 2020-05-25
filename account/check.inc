#include <ncs/chat>

void StartCheck(int client)
{
	CreateTimer(60.0, Timer_Check, client, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

public Action Timer_Check(Handle timer, int client)
{
	if (g_Accounts[client].Valid())
		return Plugin_Stop;

	NCS_Chat(client, _, "{blue}你的用户数据未加载！请输入{red} !reload_account {blue}进行加载！");
	return Plugin_Continue;
}