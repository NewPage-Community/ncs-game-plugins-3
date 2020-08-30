//#define DEV
#pragma semicolon 1

#include <ncs>

#include <ncs/account>
#include <ncs/chat>
#include <ncs/vip>

#include <sdktools>

#define P_NAME P_PRE ... " - Title"
#define P_DESC "Title management plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
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
	InitAPI();
	InitCmd();
}

public void OnPluginEnd()
{
	CloseAPI();
}

public void NCS_Account_OnUserLoaded(int client, const char[] uid)
{
	RequreTitle(client, uid);
}

public void NCS_Account_OnChangeName(int client, const char[] newname)
{
	TitleBeSet(client, playerTitle[client].type);
}

public Action OnClientSayCommand(int client, const char[] command, const char[] sArgs)
{
	if(playerTitle[client].isWaitingForplayerTitle)
	{
		if (StrLenMB(sArgs) > 4)
		{
			NCS_Chat(client, _, "{blue}自定义头衔最大长度为4，请重新输入");
			return Plugin_Stop;
		}
		playerTitle[client].isWaitingForplayerTitle = false;
		SetplayerTitle(client, sArgs);
		return Plugin_Stop;
	}
	return Plugin_Continue;
}

public void OnClientDisconnect(int client)
{
	playerTitle[client].isWaitingForplayerTitle = false;
}