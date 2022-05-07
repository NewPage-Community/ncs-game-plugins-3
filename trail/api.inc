#define BackpackItems_Service "backpack/items"
#define BackpackUser_Service "backpack/user"



void InitAPI()
{
	InitNCSAPI();
}

void CloseAPI()
{

}

void LoadTrail()
{
	static char path[128];
	Format(path, sizeof(path), "%s/type/2", BackpackItems_Service);
	NewNCSAPI(path).Get(LoadTrailInfo);
}

public void LoadTrailInfo(HTTPResponse response, DataPack dp, const char[] error)
{
	if (CheckAPIError("Trail", "LoadTrailInfo", response, error))
		return;

	// JSON
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONArray items = view_as<JSONArray>(data.Get("items"));
	int length = items.Length;

	// Loading trails
	if (trails != INVALID_HANDLE)
		delete trails;
	trails = new TrailIndex();

	for (int i = 0; i < length; i++)
	{
		JSONObject item = view_as<JSONObject>(items.Get(i));
		trails.Add(item);
		delete item;
	}
	
	delete items;
}

void CheckUserHaveTrail(int client)
{
	static char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
		return;

	int id = g_UsersUsedTrail[client];
	
	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteCell(id, false);
	
	static char path[128];
	Format(path, sizeof(path), "%s/have/%s/%d", BackpackUser_Service, uid, id);
	NewNCSAPI(path).Get(CheckUserHaveItemCallback, dp);
}

public void CheckUserHaveItemCallback(HTTPResponse response, DataPack dp, const char[] error)
{
	if (CheckAPIError("Trail", "CheckUserHaveItemCallback", response, error))
		return;

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	int id = dp.ReadCell();
	dp.Close();

	if (!IsValidClient(client))
		return;
	
	 // JSON
	JSONObject data = view_as<JSONObject>(response.Data);
	bool have = data.GetBool("have");

	if (g_UsersUsedTrail[client] == id && !have)
		g_UsersUsedTrail[client] = 0;
}