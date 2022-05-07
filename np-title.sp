#include <ncs>
#include <ncs/account>
#include <ncs/vip>
#include <ncs/cookie>
#include <sdktools>

#undef REQUIRE_PLUGIN
#include <ncs/chat>
#define REQUIRE_PLUGIN

#define P_NAME P_PRE ... " - Title"
#define P_DESC "Title management plugin"

public Plugin myinfo = 
{
	name		= P_NAME,
	author	  = P_AUTHOR,
	description = P_DESC,
	version	 = P_VERSION,
	url		 = P_URLS
};

#include "title/title"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	// lib
	RegPluginLibrary("NCS-Title");

	return APLRes_Success;
}

public void OnPluginStart()
{
	InitCmd();
}

public void NCS_Cookie_OnUserCached(int client)
{
	InitPlayerTitle(client);
}

public void NCS_Account_OnChangeName(int client, const char[] newname)
{
	SetPlayerTitle(client);
}

public Action OnClientSayCommand(int client, const char[] command, const char[] sArgs)
{
	if(player[client].isWaitingForPlayerTitle)
	{
		if (StrLenMB(sArgs) > 4)
		{
			NCS_Chat(client, _, "自定义头衔最大长度为4，请重新输入");
			return Plugin_Stop;
		}
		player[client].isWaitingForPlayerTitle = false;
		UpdatePlayerTitleCustom(client, sArgs);
		return Plugin_Stop;
	}
	return Plugin_Continue;
}

public void OnClientDisconnect(int client)
{
	player[client].type = 0;
	player[client].isWaitingForPlayerTitle = false;
}