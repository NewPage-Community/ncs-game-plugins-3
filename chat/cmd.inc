void InitCmd()
{
    RegServerCmd("ncs_chat_notify", ChatNotifyCallback, "[NCS] 聊天通知");
}

public Action ChatNotifyCallback(int args)
{
    if (!allowAllChat.BoolValue)
        return Plugin_Handled;

    char uid[MAX_UID_LENGTH], prefix[64], message[512];
    GetCmdArg(1, uid, sizeof(uid));
    GetCmdArg(2, prefix, sizeof(prefix));
    GetCmdArg(3, message, sizeof(message));

    int client = NCS_Account_GetClientOfUID(uid);

    // Invalid client
    if (!StrEqual(uid, "0") && !IsValidClient(client))
        return Plugin_Handled;

    NCS_Chat(client, prefix, message);

    return Plugin_Handled;
}
