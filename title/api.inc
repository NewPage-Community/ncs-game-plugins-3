#define  Title_Service "user/title"

HTTPClient httpClient;

void InitAPI()
{
	httpClient = NewNCSAPI();
}

void CloseAPI()
{
	delete httpClient;
}

void RequreTitle(int client, const char[] uid)
{
	if (!IsValidClient(client))
		return;

	playerTitle[client].type = 0;
	playerTitle[client].isWaitingForplayerTitle = false;

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);

	char path[128];
	Format(path, sizeof(path), "%s/%s", Title_Service, uid);
	httpClient.Get(path, RequreTitle_Resp, dp);

}

void RequreTitle_Resp(HTTPResponse response, DataPack dp, const char[] error)
{
	if (CheckAPIError("Title", "LoadTitleInfo", response, error))
		return;

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[MAX_UID_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	dp.Close();

	// JSON
	char iUID[MAX_UID_LENGTH];
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONObject info = view_as<JSONObject>(data.Get("info"));

	// Check UID is valid
	info.GetString("uid", iUID, sizeof(iUID));
	if (!StrEqual(iUID, uid) || !IsValidClient(client))
	{
		delete info;
		return;
	}

	char title[MAX_TITLE_LENGTH];
	info.GetString("custom_title", title, sizeof(title));
	playerTitle[client].type = info.GetInt("title_type");
	playerTitle[client].title = title;

	CreateTimer(1.0, FirstTitleSet, client);

	delete info;
}

void SetTitle(int client)
{
	char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
		return;
	
	// JSON
	JSONObject info = new JSONObject();
	info.SetString("custom_title", playerTitle[client].title);
	info.SetInt("title_type", playerTitle[client].type);

	JSONObject data = new JSONObject();
	data.Set("info", info);
	delete info;

	char path[128];
	Format(path, sizeof(path), "%s/%s", Title_Service, uid);
	httpClient.Put(path, data, BlankCallback);
}

public Action FirstTitleSet(Handle timer, int client)
{
	TitleBeSet(client, playerTitle[client].type);
	return Plugin_Stop;
}