void InitCmd()
{
    RegServerCmd("ncs_chat_notify", ChatNotifyCallback, "[ncs] 聊天通知");
}

public Action ChatNotifyCallback(int args)
{
    char uid[MAX_UID_LENGTH], prefix[64], context[512];
    GetCmdArg(1, uid, sizeof(uid));
    GetCmdArg(2, prefix, sizeof(prefix));
    GetCmdArg(3, context, sizeof(context));

    int client = NCS_Account_GetClientOfUID(uid);
    NCS_Chat(client, prefix, context);

    return Plugin_Handled;
}
