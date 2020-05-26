#define Server_Service "user/account"

HTTPClient httpClient;

Handle onLoaded = INVALID_HANDLE;

void InitAPI()
{
	httpClient = NewNCSAPI();
}

void RegForward()
{
	onLoaded = CreateGlobalForward("NCS_Account_OnLoaded",  ET_Ignore, Param_Cell, Param_Cell);
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

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteString(steamid, false);
	dp.WriteCell(notify, false);

	char path[128];
	Format(path, sizeof(path), "%s/GetUID/%s", Server_Service, steamid);
	httpClient.Get(path, LoadAccountUID, dp);
}

public void LoadAccountUID(HTTPResponse response, DataPack dp, const char[] error)
{
	if (response.Status != HTTPStatus_OK || response.Data == null)
	{
		NCS_LogError("Account", "LoadAccountUID", "Faild to load info %d (%s)", response.Status, error);
		return;
	}

	// Load data
	char steamid[32];
	dp.Reset();
	dp.ReadString(steamid, sizeof(steamid));
	bool notify = dp.ReadCell();
	dp.Close()
	JSONObject data = view_as<JSONObject>(response.Data);
	
	int client = FindClientBySteamId(AuthId_SteamID64, steamid);
	// Invalid
	if (client == 0)
		return;

	g_Accounts[client].index = client;
	g_Accounts[client].uid = data.GetInt("uid");
	g_Accounts[client].SetSteamID(steamid);

	// Call global forward
	Call_StartForward(onLoaded);
	Call_PushCell(client);
	Call_PushCell(g_Accounts[client].uid);
	Call_Finish();

	if (notify)
		NCS_Chat(client, _, "{blue}你的用户数据已加载成功！");
}
