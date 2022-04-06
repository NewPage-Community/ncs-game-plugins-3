void InitCmd()
{
    RegCmd();
}

void RegCmd()
{
    RegServerCmd("ncs_cookie_notify", CookieNotifyCallback, "[ncs] Cookie通知");
}

public Action CookieNotifyCallback(int args)
{
    static char uid[MAX_UID_LENGTH];
    GetCmdArg(1, uid, sizeof(uid));

    int client = NCS_Account_GetClientOfUID(uid);
    if (client > 0)
        LoadUserCookie(client, uid);

    return Plugin_Handled;
}
