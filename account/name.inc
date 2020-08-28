#include <sdktools_functions>

char g_LastName[MAXPLAYERS + 1][MAX_NAME_LENGTH];

GlobalForward onChangeName;

void InitName()
{
	onChangeName = new GlobalForward("NCS_Account_OnChangeName",  ET_Ignore, Param_Cell, Param_String);
	HookNameEvent();
}

void HookNameEvent()
{
	//This hooks the event, we'll implement a listener for it.
	bool exists = HookEventEx("player_changename", Event_NameChanged, EventHookMode_Pre);
	if(!exists)
		NCS_LogError("Account", "HookNameEvent", "Faild to hook player_changename");
}

public Action Event_NameChanged(Event event, const char[] name, bool dontBroadcast)
{
	// This gets the client's user ID then translates it to a client index
	int client = GetClientOfUserId(event.GetInt("userid"));

	// Validity of client
	if(client > MaxClients || client < 1) return Plugin_Continue;
	if(!IsClientConnected(client) || !IsClientInGame(client)) return Plugin_Continue;

	// Get the new name of client
	char newName[MAX_NAME_LENGTH];
	event.GetString("newname", newName, sizeof(newName));

	// Use Safety ChangeName to Pass
	if (StrEqual(g_LastName[client], newName))
		return Plugin_Continue;
	// Update
	g_LastName[client] = newName;
	
	// Account service
	if (g_Accounts[client].Valid())
	{
		g_Accounts[client].UpdateName(newName);
		UpdateAccountName(client);
	}

	// Start function call
	Call_StartForward(onChangeName);
	Call_PushCell(client);
	Call_PushString(newName);
	Call_Finish();

	return Plugin_Continue;
}

void ChangeName(int client, const char[] name)
{
	if (!IsValidClient(client))
		return;

	Format(g_LastName[client], MAX_NAME_LENGTH, "%s", name);
	SetClientName(client, g_LastName[client]);
}
