#define Admin_Service "user/admin"

void InitAPI()
{
	InitNCSAPI();
}

void CloseAPI()
{

}

void ReqAdmin(int client, const char[] uid)
{
	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);

	static char path[128];
	Format(path, sizeof(path), "%s/%s", Admin_Service, uid);
	NewNCSAPI(path).Get(LoadAdminInfo, dp);
}

public void LoadAdminInfo(HTTPResponse response, DataPack dp, const char[] error)
{
	if (CheckAPIError("Admin", "LoadAdminInfo", response, error))
		return;

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	static char uid[MAX_UID_LENGTH];
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

	int flagbits = info.GetInt("flag");
	int immunity = info.GetInt("immunity");

	if (flagbits > 0 || immunity > 0)
		LoadAdmin(client, flagbits, immunity);

	delete info;
}

void ReloadAllAdmins()
{
	for (int i = 0; i <= MAXPLAYERS; i++)
	{
		static char uid[MAX_UID_LENGTH];
		if (!NCS_Account_GetUID(i, uid, sizeof(uid)))
			continue;
		ReqAdmin(i, uid);
	}
}