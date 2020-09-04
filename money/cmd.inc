void InitCmd()
{
    RegConsoleCmd("sm_moneyrecords", Command_MoneyRecords, "[NCS] 软妹币流水账");
}

public Action Command_MoneyRecords(int client, int args)
{
    DisplayMoneyRecord(client);
    return Plugin_Handled;
}