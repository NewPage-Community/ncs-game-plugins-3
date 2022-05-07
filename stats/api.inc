

static Handle updateStats;

void InitAPI()
{
	InitNCSAPI();
	StatsList = new Stats();
	updateStats = CreateTimer(60.0, Timer_UpdateStats, 0, TIMER_REPEAT);
}

void CloseAPI()
{
	delete updateStats;

	UpdateStatsData();
	

	delete StatsList;
}

void UpdateStatsData()
{
	// Do not send empty data
	if (StatsList.Length == 0)
		return;

	JSONObject data = StatsList.JSON();
	StatsList.Clear();

	static char path[128];
	Format(path, sizeof(path), "%s/user/incrs", Stats_Service);
	NewNCSAPI(path).Post(data, UpdateStatsDataCallback);
	delete data;
}

public void UpdateStatsDataCallback(HTTPResponse response, any value, const char[] error)
{
	CheckAPIError("Stats", "UpdateStatsDataCallback", response, error);
}

public Action Timer_UpdateStats(Handle timer)
{
	UpdateStatsData();
	return Plugin_Continue;
}

void GetStatsData(int client, Handle plugin, Function callback, const char[] range, const char[] statsName, const char[] version, any data)
{
	static char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, MAX_UID_LENGTH))
		return;

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client);
	dp.WriteString(uid);
	dp.WriteCell(plugin);
	dp.WriteFunction(callback);
	dp.WriteCell(data);

	static char path[128];
	Format(path, sizeof(path), "%s/user/%s/%s/%s/%s", Stats_Service, uid, statsName, version, range);
	NewNCSAPI(path).Get(GetStatsDataCallback, dp);
}

public void GetStatsDataCallback(HTTPResponse response, DataPack dp, const char[] error)
{
	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[MAX_UID_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	Handle plugin = dp.ReadCell();
	Function callback = dp.ReadFunction();
	any cdata = dp.ReadCell();
	dp.Close();

	if (CheckAPIError("Stats", "GetStatsDataCallback", response, error))
		return;

	// JSON
	JSONObject data = view_as<JSONObject>(response.Data);
	float score = data.GetFloatEx("score");
	int rank = data.GetStringInt("rank");
	int total = data.GetStringInt("total");

	Call_StartFunction(plugin, callback);
	Call_PushCell(client);
	Call_PushCell(score);
	Call_PushCell(rank);
	Call_PushCell(total);
	Call_PushCell(cdata);
	Call_Finish();
}

void GetGlobalStatsData(Handle plugin, Function callback, const char[] range, const char[] statsName, const char[] version, any data)
{
	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(plugin);
	dp.WriteFunction(callback);
	dp.WriteCell(data);

	static char path[128];
	Format(path, sizeof(path), "%s/global/%s/%s/%s", Stats_Service, statsName, version, range);
	NewNCSAPI(path).Get(GetGlobalStatsDataCallback, dp);
}

public void GetGlobalStatsDataCallback(HTTPResponse response, DataPack dp, const char[] error)
{
	// DataPack
	dp.Reset();
	Handle plugin = dp.ReadCell();
	Function callback = dp.ReadFunction();
	any cdata = dp.ReadCell();
	dp.Close();

	if (CheckAPIError("Stats", "GetGlobalStatsDataCallback", response, error))
		return;

	// JSON
	JSONObject data = view_as<JSONObject>(response.Data);
	float score = data.GetFloatEx("score");

	Call_StartFunction(plugin, callback);
	Call_PushCell(score);
	Call_PushCell(cdata);
	Call_Finish();
}

void AddGlobalStatsData(const char[] range, const char[] statsName, const char[] version, float increment)
{
	// JSON data
	JSONObject data = new JSONObject();
	data.SetFloat("increment", increment);

	static char path[128];
	Format(path, sizeof(path), "%s/global/incr/%s/%s/%s", Stats_Service, statsName, version, range);
	NewNCSAPI(path).Post(data, AddGlobalStatsDataCallback);
	delete data;
}

public void AddGlobalStatsDataCallback(HTTPResponse response, any value, const char[] error)
{
	CheckAPIError("Stats", "AddGlobalStatsDataCallback", response, error);
}