#include <ncs>
#include <ncs/server>
#include <ncs/account>
#include <ncs/vip>
#include <ncs/sign>
#include <ncs/backpack>
#include <ncs/chat>
#include <ncs/stats>
#include <ncs/pass>
#include <ncs/money>
#include <ncs/cookie>

#define P_NAME P_PRE ... " - Test"
#define P_DESC "Test plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
};

public void OnPluginStart()
{
    RegConsoleCmd("sm_testall", Command_testall);
}

public Action Command_testall(int client, int args)
{
    // Account
    char uid[MAX_UID_LENGTH];
    NCS_Account_GetUID(client, uid, sizeof(uid));
    NCS_Chat(client, "TEST", "uid: %s", uid);
    NCS_Account_ChangeName(client, "Test");

    // VIP
    NCS_Chat(client, "TEST", "Is VIP: %d", NCS_VIP_IsVIP(client));
    NCS_Chat(client, "TEST", "VIP level: %d", NCS_VIP_Level(client));
    NCS_Chat(client, "TEST", "Now add 1 point and renewal 1 day");
    NCS_VIP_AddPoint(client, 1);
    NCS_VIP_Renewal(client, 3600 * 24);
    NCS_Chat(client, "TEST", "Is VIP: %d", NCS_VIP_IsVIP(client));
    NCS_Chat(client, "TEST", "VIP level: %d", NCS_VIP_Level(client));

    // Server
    NCS_Chat(client, "TEST", "server_id: %d", NCS_Server_GetID());
    NCS_Chat(client, "TEST", "mod_id: %d", NCS_Server_GetModID());
    NCS_Chat(client, "TEST", "game_id: %d", NCS_Server_GetGameID());

    // Backpack
    Item item;
    item.id = 1;
    item.amount = 1;
    Items items = new Items();
    items.Add(item);
    NCS_Backpack_AddItems(client, items);
    delete items;

    // Chat
    NCS_Chat_SetTitle(client, "test", "{red}");
    NCS_Chat_SetNameColor(client, "{red}");

    // Stats
    NCS_Stats_Add(client, "all", "test", "v1", 1.0);

    // Pass
    NCS_Pass_AddPoint(client, 100);

    // Money
    NCS_Money_Give(client, 100, "test");

    // Cookie
    NCS_Cookie_Set(client, "test", "test");
    return Plugin_Handled;
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

public void NCS_Sign_OnUserSigned(int client)
{
    PrintToServer("NCS_Sign_OnUserSigned(client:%d)", client);
}

public void NCS_Cookie_OnUserCached(int client)
{
    char buffer[MAX_COOKIE_VALUE_LENGTH];
    PrintToServer("NCS_Cookie_OnUserCached(client:%d)", client);
    bool ok = NCS_Cookie_Get(client, "test", buffer, sizeof(buffer));
    PrintToServer("Cookie test(%d): %s", ok, buffer);
}