void InitCmd()
{
	RegServerCmd("ncs_chat_notify", ChatNotifyCallback, "[NCS] 聊天通知");
	RegConsoleCmd("sm_allchat", AllChatSetting, "[NCS] 全服聊天开关");
}

public Action ChatNotifyCallback(int args)
{
	char uid[MAX_UID_LENGTH], prefix[64], message[512];
	GetCmdArg(1, uid, sizeof(uid));
	GetCmdArg(2, prefix, sizeof(prefix));
	GetCmdArg(3, message, sizeof(message));

	int client = NCS_Account_GetClientOfUID(uid);
	bool allchat = StrEqual(uid, "0");

	if (allchat)
	{
		for (int i = 1; i <= MaxClients; i++)
			if (IsValidClient(i))
				if (!GetCookieAllChatDisabled(i))
					NCS_Chat(i, prefix, message);
	}
	else
	{
		if (IsValidClient(client))
			NCS_Chat(client, prefix, message);
	}

	return Plugin_Handled;
}

public Action AllChatSetting(int client, int args)
{
	if (!IsValidClient(client))
		return Plugin_Handled;

	bool disabled = !GetCookieAllChatDisabled(client);

	SetCookieAllChatDisabled(client, disabled);
	NCS_Chat(client, _, "全服聊天已%s", disabled ? "关闭" : "打开");
	return Plugin_Handled;
}