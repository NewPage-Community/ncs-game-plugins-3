#define Account_Service "user/account"

HTTPClient httpClient;

GlobalForward onUserLoaded;
GlobalForward onUserDisconnect;

void InitAPI()
{
	httpClient = NewNCSAPI();
	RegAPIForward();
}

void RegAPIForward()
{
	onUserLoaded = new GlobalForward("NCS_Account_OnUserLoaded",  ET_Ignore, Param_Cell, Param_String);
	onUserDisconnect = new GlobalForward("NCS_Account_OnUserDisconnect", ET_Ignore, Param_Cell, Param_String);
}

void CloseAPI()
{
	delete httpClient;
}

void ReqAccountUID(const char[] steamid, bool notify = false)
{
	if (strlen(steamid) <= 0)
		return;

	int client = FindClientBySteamId(AuthId_SteamID64, steamid);
	if (client == 0)
		return;
	
	// Clean
	g_Accounts[client].Clean();

	// Username
	char username[MAX_NAME_LENGTH];
	GetClientName(client, username, MAX_NAME_LENGTH);

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteString(steamid, false);
	dp.WriteCell(notify, false);

	// JSON data
	JSONObject data = new JSONObject();
	data.SetInt64("steam_id", steamid);
	data.SetString("username", username);

	char path[128];
	Format(path, sizeof(path), "%s/game/playerconnect", Account_Service);
	httpClient.Post(path, data, LoadAccountUID, dp);
}

public void LoadAccountUID(HTTPResponse response, DataPack dp, const char[] error)
{
	if (CheckAPIError("Account", "LoadAccountUID", response, error))
		return;

	// Load data
	char steamid[32];
	dp.Reset();
	dp.ReadString(steamid, sizeof(steamid));
	bool notify = dp.ReadCell();
	dp.Close()
	JSONObject data = view_as<JSONObject>(response.Data);
	
	int client = FindClientBySteamId(AuthId_SteamID64, steamid);
	// Invalid
	if (!IsValidClient(client))
		return;

	char uid[MAX_UID_LENGTH];
	data.GetString("uid", uid, sizeof(uid));

	// Init user
	g_Accounts[client].Init(client, uid, steamid);
	// UIDMap
	InitClient(client, uid);

	// Call global forward
	Call_StartForward(onUserLoaded);
	Call_PushCell(client);
	Call_PushString(uid);
	Call_Finish();

	if (notify)
		NCS_Chat(client, _, "你的用户数据已加载成功");
}

void UpdateAccountName(int client)
{
	if (!g_Accounts[client].Valid())
		return;

	JSONObject data = new JSONObject();
	data.SetString("username", g_Accounts[client].name);

	char path[128];
	Format(path, sizeof(path), "%s/%s", Account_Service, g_Accounts[client].uid);
	httpClient.Put(path, data, UpdateAccountNameCallback);
}

public void UpdateAccountNameCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Account", "UpdateAccountNameCallback", response, error);
}

void UserDisconnectForward(int client, const char[] uid)
{
	// Call global forward
	Call_StartForward(onUserDisconnect);
	Call_PushCell(client);
	Call_PushString(uid);
	Call_Finish();
}