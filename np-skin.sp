#include <ncs>
#include <ncs/account>
#include <ncs/cookie>
#include <ncs/game>
#include <sdkhooks>
#include <sdktools>
#include <regex>

#undef REQUIRE_PLUGIN
#include <ncs/chat>
#include <zombiereloaded>
#define REQUIRE_PLUGIN

ConVar cv_skin_only_team;

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

	MarkNativeAsOptional("ZR_IsClientZombie");

	return APLRes_Success;
}

public void OnAllPluginsLoaded()
{
	InitLibrary();
}

public void OnPluginStart()
{
	InitAPI();
	InitCmd();
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
	HookEvent("player_death", Event_PlayerDeath_Pre, EventHookMode_Pre);
	HookEvent("round_end", Event_RoundEnd, EventHookMode_Post);
	RegConsoleCmd("InsRadial", RadialCommand);

	cv_skin_only_team = CreateConVar("skin_only_team", "0", "", 0, true, 0.0, true, 3.0);
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

public void NCS_Cookie_OnUserCached(int client)
{
	SkinRadioBlockGetCookie(client);
	SkinRadioVolumeGetCookie(client);
	//SkinRadioOffGetCookie(client);
	//SkinArmsOffGetCookie(client);
	GetUsedSkin(client);
}

public Action Event_PlayerDeath_Pre(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(event.GetInt("userid"));
	if (IsFakeClient(client) || !IsClientInGame(client))
		return Plugin_Continue;
	SkinSoundOnPlayerDeath(client);
	return Plugin_Continue;
}

public Action RadialCommand(int client, int args) {
	if (!IsClientInGame(client))
		return Plugin_Handled;
	static char arg[32];
	GetCmdArg(1, arg, sizeof(arg));
	SkinSoundOnPlayerRadio(client, arg);
	return Plugin_Handled;
}

public Action Event_RoundEnd(Event event, const char[] name, bool dontBroadcast)
{
	int winner = GetEventInt(event, "winner");
	SkinSoundOnRoundEnd(winner);
	return Plugin_Continue;
}