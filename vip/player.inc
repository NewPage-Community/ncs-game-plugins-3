void PlayerInit(int client)
{
    if (!g_VIPs[client].IsVIP())
        return;
    
    char name[MAX_NAME_LENGTH];
    NCS_Account_GetName(client, name, sizeof(name));
    NCS_Chat(_, _, "{blue}欢迎 {green}%s {red}(%d级VIP) 进入服务器", name, g_VIPs[client].Level());
}