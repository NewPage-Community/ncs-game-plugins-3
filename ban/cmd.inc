void InitCmd()
{
    RegCmd();
}

void RegCmd()
{
    RegServerCmd("ncs_ban_notify", BanNotifyCallback, "[ncs] 封禁通知");
}

public Action BanNotifyCallback(int args)
{
    char uid[MAX_UID_LENGTH], type[2], reason[256];
    GetCmdArg(1, uid, sizeof(uid));
    GetCmdArg(2, type, sizeof(type));
    GetCmdArg(3, reason, sizeof(reason));

    int client = GetClientFromUID(uid);
    if (client > 0)
        ReqBanCheck(client, uid);

    NCS_Chat(_, _, "{red}玩家 %s (UID) 已被%s 原因：%s", uid, g_banType[StringToInt(type) == 1 ? 1 : 0], reason);

    return Plugin_Handled;
}
