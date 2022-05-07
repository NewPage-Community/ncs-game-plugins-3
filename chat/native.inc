GlobalForward onTitleChanged;

void InitNative()
{
	CreateNative("NCS_Chat_SetTitle", Native_SetTitle);
	CreateNative("NCS_Chat_SetNameColor", Native_SetNameColor);
	CreateNative("NCS_Chat_GetChatName", Native_GetChatName);
	CreateNative("NCS_Chat_GetTitle", Native_GetTitle);
	onTitleChanged = new GlobalForward("NCS_Chat_OnTitleChanged", ET_Ignore, Param_Cell, Param_String);
}

public int Native_SetTitle(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	if (!player[client].IsValid())
		return 1;

	char title[MAX_TITLE_LENGTH], titleColor[MAX_COLOR_LENGTH];
	GetNativeString(2, title, sizeof(title));
	GetNativeString(3, titleColor, sizeof(titleColor));

	player[client].SetTitle(title, titleColor);

	Call_StartForward(onTitleChanged);
	Call_PushCell(client);
	Call_PushString(title);
	Call_Finish();

	return 1;
}

public int Native_SetNameColor(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	if (!player[client].IsValid())
		return 1;

	char nameColor[MAX_COLOR_LENGTH];
	GetNativeString(2, nameColor, sizeof(nameColor));

	player[client].SetNameColor(nameColor);
	return 1;
}

public int Native_GetChatName(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int size = GetNativeCell(3);
	if (!player[client].IsValid())
		return 1;

	char name[MAX_NAME_LENGTH_FIX];
	player[client].GetChatName(name, sizeof(name));
	SetNativeString(2, name, size);
	return 1;
}

public int Native_GetTitle(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int size = GetNativeCell(3);
	if (!player[client].IsValid())
		return 1;

	SetNativeString(2, player[client].title, size);
	return 1;
}