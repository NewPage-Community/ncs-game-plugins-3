void InitCmd()
{
    RegConsoleCmd("sm_moneyrecords", Command_MoneyRecords, "[NCS] 软妹币流水账");
    RegConsoleCmd("sm_gift", Command_MoneyGift, "[NCS] 赠送软妹币");
}

public Action Command_MoneyRecords(int client, int args)
{
    DisplayMoneyRecord(client);
    return Plugin_Handled;
}

public Action Command_MoneyGift(int client, int args)
{
    static char buffer[8];
    GetCmdArgString(buffer, sizeof(buffer));
    int money = StringToInt(buffer);
    if (money <= 0) {
        ReplyToCommand(client, "用法: sm_gift <金额>");
        return Plugin_Handled;
    }

    if (money > 200)
    {
        ReplyToCommand(client, "赠送失败: 赠送不得超过 200 软妹币");
        return Plugin_Handled;
    }

    GiftMoneyMenu(client, money)
    return Plugin_Handled;
}
