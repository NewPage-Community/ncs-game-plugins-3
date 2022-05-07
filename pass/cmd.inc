void InitCmd()
{
	RegConsoleCmd("sm_pass", Command_PassMenu, "[NCS] 通行证菜单");
	RegServerCmd("ncs_pass_upgrade_notify", UpgradeNotify, "[NCS] 通行证升级通知");
}

public Action Command_PassMenu(int client, int args)
{
	DisplayPassInfo(client);
	return Plugin_Handled;
}

public Action UpgradeNotify(int args)
{
	char uid[MAX_UID_LENGTH], clevel[16];
	GetCmdArg(1, uid, sizeof(uid));
	GetCmdArg(2, clevel, sizeof(clevel));

	int client = NCS_Account_GetClientOfUID(uid);
	int level = StringToInt(clevel) + 1;

	// Invalid client
	if (!StrEqual(uid, "0") && !IsValidClient(client))
		return Plugin_Handled;

	if (level >= MAX_LEVEL)
		return Plugin_Handled;

	char name[MAX_ITEM_NAME_LENGTH];
	char nextRewards[512];
	bool firstReward = true;
	// Free
	if (rewards[level][FreeReward].IsValid())
	{
		rewards[level][FreeReward].GetName(name, sizeof(name));
		Format(nextRewards, sizeof(nextRewards), firstReward ? "%s%s" : "%s，%s", nextRewards, name);
		firstReward = false;
	}
	// Adv
	if (rewards[level][AdvReward].IsValid())
	{
		rewards[level][AdvReward].GetName(name, sizeof(name));
		Format(nextRewards, sizeof(nextRewards), firstReward ? "%s%s" : "%s，%s", nextRewards, name);
		firstReward = false;
	}
	// No things
	if (firstReward)
		strcopy(nextRewards, sizeof(nextRewards), "无");

	// Chat notify
	NCS_Chat(client, _, "通行证成功升级到 {green}%d{default} 级！\n购买高级通行证可解锁更多大奖～\n下一级奖励：{green}%s", level - 1, nextRewards);
	return Plugin_Handled;
}