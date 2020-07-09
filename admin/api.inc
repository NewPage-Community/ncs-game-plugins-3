#define Admin_Service "user/admin"

HTTPClient httpClient;

void InitAPI()
{
	httpClient = NewNCSAPI();
}

void CloseAPI()
{
	delete httpClient;
}

void ReqAdmin(int client, const char[] uid)
{
	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);

	char path[128];
	Format(path, sizeof(path), "%s/%s", Admin_Service, uid);
	httpClient.Get(path, LoadAdminInfo, dp);
}

public void LoadAdminInfo(HTTPResponse response, DataPack dp, const char[] error)
{
	// Admin record not found
	if (response.Status == HTTPStatus_NotFound)
		return;

	if (response.Status != HTTPStatus_OK || response.Data == null)
	{
		NCS_LogError("Admin", "LoadAdminInfo", "Faild to load info %d (%s)", response.Status, error);
		return;
	}

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[INT64_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	dp.Close()

	// JSON
	char iUID[INT64_LENGTH];
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONObject info = view_as<JSONObject>(data.Get("info"));

	// Check UID is valid
	info.GetString("uid", iUID, sizeof(iUID));
	if (!StrEqual(iUID, uid))
		return;

	int flagbits = info.GetInt("flag");
	int immunity = info.GetInt("immunity");

	LoadAdmin(client, flagbits, immunity);
}