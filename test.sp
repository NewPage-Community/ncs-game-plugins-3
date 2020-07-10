#include <ncs/server>
#include <ncs/account>
#include <ncs/vip>

public Plugin myinfo = 
{
    name        = "P_NAME",
    author      = "P_AUTHOR",
    description = "P_DESC",
    version     = "P_VERSION",
    url         = "P_URLS"
};

public OnPluginStart()
{
    RegConsoleCmd("sm_testall", Command_testall);
    RegConsoleCmd("sm_testvip", Command_testvip);
}

public Action Command_testall(int client, int args)
{
    // Account
    char uid[MAX_UID_LENGTH];
    NCS_Account_GetUID(client, uid, sizeof(uid));
    PrintToChat(client, "uid: %s", uid);
    NCS_Account_ChangeName(client, "Test");

    // VIP
    PrintToChat(client, "Is VIP: %s", NCS_VIP_IsVIP(client));
    PrintToChat(client, "VIP level: %s", NCS_VIP_Level(client));

    // Server
    PrintToChat(client, "server_id: %d", NCS_Server_GetID());
    PrintToChat(client, "mod_id: %d", NCS_Server_GetModID());
    PrintToChat(client, "game_id: %d", NCS_Server_GetGameID());
}

public Action Command_testvip(int client, int args)
{
    NCS_VIP_AddPoint(client, 1);
    NCS_VIP_Renewal(client, 3600 * 24);
}

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
    PrintToServer("NCS_Account_OnLoaded(client:%d, uid:%s)", client, uid);
}

public void NCS_Account_OnChangeName(int client, const char[] newname)
{
    PrintToServer("NCS_Account_OnChangeName(client:%d, newname:%s)", client, newname);
}

public void NCS_Server_OnLoaded()
{
    PrintToServer("NCS_Server_OnLoaded()");
}