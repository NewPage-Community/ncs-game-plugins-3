#include <ncs/chat>

void RegCmd()
{
	RegConsoleCmd("sm_reload_account", Command_ReloadAccount, "[NCS] 重载用户数据");
}

public Action Command_ReloadAccount(int client, int args)
{
	if (g_Accounts[client].Valid())
	{
		NCS_Chat(client, _, "{blue}你的用户数据已成功加载，请勿重复操作！");
		return Plugin_Handled;
	}

	char steamid[32];
	GetClientAuthId(client, AuthId_SteamID64, steamid, sizeof(steamid));
	ReqAccountUID(steamid, true);
	return Plugin_Handled;
}
