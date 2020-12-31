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
    int level = StringToInt(clevel);

    // Invalid client
    if (!StrEqual(uid, "0") && !IsValidClient(client))
        return Plugin_Handled;

    char nextRewards[512];
    // Free
    for (int i = 0; i < MAX_LEVEL_REWARDS && rewards[level][FreeReward][i].IsValid(); i++)
    {
        char name[MAX_ITEM_NAME_LENGTH];
        rewards[level][FreeReward][i].GetName(name, sizeof(name));
        Format(nextRewards, sizeof(nextRewards), i == 0 ? "%s%s" : "%s,%s", nextRewards, name);
    }
    // Adv
    for (int i = 0; i < MAX_LEVEL_REWARDS && rewards[level][AdvReward][i].IsValid(); i++)
    {
        char name[MAX_ITEM_NAME_LENGTH];
        rewards[level][AdvReward][i].GetName(name, sizeof(name));
        Format(nextRewards, sizeof(nextRewards), "%s,%s", nextRewards, name);
    }
    // No things
    if (rewards[level][FreeReward][0].IsValid() && rewards[level][AdvReward][0].IsValid())
        strcopy(nextRewards, sizeof(nextRewards), "无");

    // Chat notify
    NCS_Chat(client, _, "{blue}通行证成功升级到 {green}%d{blue} 级，购买高级通行证可解锁更多大奖～", level);
    NCS_Chat(client, _, "{blue}下一级奖励：{green}%s", nextRewards);
    return Plugin_Handled;
}