#include <ncs>
#include <ncs/account>
#include <ncs/cookie>
#include <ncs/game>
#include <sdkhooks>
#include <sdktools>
#include <regex>
#include <InsExt>

#undef REQUIRE_PLUGIN
#include <ncs/chat>
#define REQUIRE_PLUGIN

#define GAME_INSURGENCY

#define P_NAME P_PRE ... " - Skin"
#define P_DESC "Skin management plugin"

public Plugin myinfo = 
{
	name		= P_NAME,
	author	  = P_AUTHOR,
	description = P_DESC,
	version	 = P_VERSION,
	url		 = P_URLS
};

// Module
#include "skin/skin"

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	InitNative();

	// lib
	RegPluginLibrary("NCS-Skin");

	return APLRes_Success;
}

public void OnPluginStart()
{
	InitAPI();
	InitCmd();
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
	HookEvent("player_death", Event_PlayerDeath_Pre, EventHookMode_Pre);
	HookEvent("round_end", Event_RoundEnd, EventHookMode_Post);
	RegConsoleCmd("InsRadial", RadialCommand);
	AddNormalSoundHook(InsSoundHook);
}

public void OnPluginEnd()
{
	CloseAPI();
}

public void OnConfigsExecuted()
{
	LoadSkins();
}

public void OnClientConnected(int client)
{
	SkinSoundOnClientConnected(client);
}

public void OnClientDisconnect(int client)
{
	ClearPlayerModel(client);
}

public void Event_PlayerSpawn(Event event, const char[] name1, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (client < 1 || client > MaxClients || !IsClientInGame(client) || IsFakeClient(client))
		return;

	Model_PlayerSpawn(client);
	Preview_PlayerSpawn(client);
	SkinSoundOnPlayerSpawn(client);

	return;
}

public Action Event_PlayerDeath_Pre(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(event.GetInt("userid"));
	if (IsFakeClient(client) || !IsClientInGame(client))
		return Plugin_Continue;
	SkinSoundOnPlayerDeath(client);
	return Plugin_Continue;
}

public void NCS_Cookie_OnUserCached(int client)
{
	SkinRadioBlockGetCookie(client);
	SkinRadioVolumeGetCookie(client);
	SkinRadioOffGetCookie(client);
	SkinArmsOffGetCookie(client);
	GetUsedSkin(client);
}

public Action RadialCommand(int client, int args) {
	if (!IsClientInGame(client))
		return Plugin_Handled;
	static char arg[32];
	GetCmdArg(1, arg, sizeof(arg));
	bool block = SkinSoundOnPlayerRadio(client, arg);
	return block ? Plugin_Handled : Plugin_Continue;
}

public Action Event_RoundEnd(Event event, const char[] name, bool dontBroadcast)
{
	int winner = GetEventInt(event, "winner");
	SkinSoundOnRoundEnd(winner);
	return Plugin_Continue;
}