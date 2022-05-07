#define VIP_Service "user/vip"



void InitAPI()
{
	InitNCSAPI();
}

void CloseAPI()
{
}

void ReqVIPInfo(int client, const char[] uid, bool reload = false)
{
	g_VIPs[client].Clean();

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);
	dp.WriteCell(reload, false);

	static char path[128];
	Format(path, sizeof(path), "%s/%s", VIP_Service, uid);
	NewNCSAPI(path).Get(LoadVIPInfo, dp);
}

public void LoadVIPInfo(HTTPResponse response, DataPack dp, const char[] error)
{
	if (CheckAPIError("VIP", "LoadVIPInfo", response, error))
		return;

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[MAX_UID_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	bool reload = dp.ReadCell();
	dp.Close();

	// JSON
	char iUID[MAX_UID_LENGTH], expireDate[INT64_LENGTH];
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONObject info = view_as<JSONObject>(data.Get("info"));

	// Check UID is valid
	info.GetString("uid", iUID, sizeof(iUID));
	if (!StrEqual(iUID, uid) || !IsValidClient(client))
	{
		delete info;
		return;
	}
	
	// Get vip information
	int point = info.GetInt("point");
	info.GetString("expire_date", expireDate, sizeof(expireDate));

	g_VIPs[client].Init(client, point, expireDate);

	// internal vip info init hook
	if (!reload)
		PlayerInit(client);

	delete info;
}

void AddPoint(int client, int addPoint)
{
	static char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
		return;

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);
	dp.WriteCell(addPoint, false);

	// JSON data
	JSONObject data = new JSONObject();
	data.SetInt("add_point", addPoint);

	static char path[128];
	Format(path, sizeof(path), "%s/%s/addpoint", VIP_Service, uid);
	NewNCSAPI(path).Put(data, AddPointCallback, dp);
	delete data;
}

public void AddPointCallback(HTTPResponse response, DataPack dp, const char[] error)
{
	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[MAX_UID_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	int addPoint = dp.ReadCell();
	dp.Close();

	if (CheckAPIError("VIP", "AddPointCallback", response, error))
	{
		NCS_LogError("VIP", "AddPointCallback", "#%d UID:%s AddPoint:%d", client, uid, addPoint);
		return;
	}

	if (!g_VIPs[client].IsValid())
		return;

	JSONObject data = view_as<JSONObject>(response.Data);
	g_VIPs[client].point = data.GetInt("point");
}

void Renewal(int client, int length)
{
	static char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
		return;

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);
	dp.WriteCell(length, false);

	// JSON data
	JSONObject data = new JSONObject();
	data.SetInt64("uid", uid);
	data.SetInt("length", length);

	static char path[128];
	Format(path, sizeof(path), "%s/renewal", VIP_Service);
	NewNCSAPI(path).Post(data, RenewalCallback, dp);
	delete data;
}

public void RenewalCallback(HTTPResponse response, DataPack dp, const char[] error)
{
	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[MAX_UID_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	int length = dp.ReadCell();
	dp.Close();

	if (CheckAPIError("VIP", "RenewalCallback", response, error))
	{
		NCS_LogError("VIP", "RenewalCallback", "#%d UID:%s Length:%d", client, uid, length);
		return;
	}

	if (!g_VIPs[client].IsValid())
		return;

	char expireDate[INT64_LENGTH];
	JSONObject data = view_as<JSONObject>(response.Data);
	data.GetString("expire_date", expireDate, sizeof(expireDate));
	g_VIPs[client].Renewal(expireDate);
}