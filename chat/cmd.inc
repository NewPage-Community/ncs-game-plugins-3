void InitCmd()
{
    RegServerCmd("ncs_chat_notify", ChatNotifyCallback, "[ncs] 聊天通知");
}

public Action ChatNotifyCallback(int args)
{
    char uid[MAX_UID_LENGTH], prefix[64], message[512];
    GetCmdArg(1, uid, sizeof(uid));
    GetCmdArg(2, prefix, sizeof(prefix));
    GetCmdArg(3, message, sizeof(message));

    int client = NCS_Account_GetClientOfUID(uid);
    NCS_Chat(client, prefix, message);

    return Plugin_Handled;
}