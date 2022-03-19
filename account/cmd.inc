#include <ncs/chat>

void InitCmd()
{
    RegCmd();
}

void RegCmd()
{
    RegConsoleCmd("sm_account", Command_Account, "[NCS] 用户信息");
}

public Action Command_Account(int client, int args)
{
    AccountMenu(client);
    return Plugin_Handled;
}
