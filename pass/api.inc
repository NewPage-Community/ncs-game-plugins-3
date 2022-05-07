#define PassUser_Service "pass/user"
#define GamePass_Service "game/pass"



static Handle updatePointTimer;

void InitAPI()
{
	InitNCSAPI();
	updatePointTimer = CreateTimer(60.0, Timer_UpdatePoint, 0, TIMER_REPEAT);
}

void CloseAPI()
{
	delete updatePointTimer;

}

public Action Timer_UpdatePoint(Handle timer, int client)
{
	JSONObject data = new JSONObject();
	JSONArray array = new JSONArray();

	int count = 0;
	if (client != 0)
	{
		if (playerPass[client].IsValid() && playerPass[client].point > 0)
		{
			playerPass[client].PushJSONArray(array);
			playerPass[client].point = 0;
			count++;
		}
	}
	else
	{
		int length = sizeof(playerPass);
		for (int i = 0; i < length; i++)
		{
			if (playerPass[i].IsValid() && playerPass[i].point > 0)
			{
				playerPass[i].PushJSONArray(array);
				count++;
			}
			playerPass[i].point = 0;
		}
	}
	
	data.Set("add", array);
	delete array;

	// Do not send empty data
	if (count > 0)
	{
		static char path[128];
		Format(path, sizeof(path), "%s/addpoints", PassUser_Service);
		NewNCSAPI(path).Post(data, UpdatePointCallback);
	}
	delete data;
	
	return Plugin_Continue;
}

public void UpdatePointCallback(HTTPResponse response, any value, const char[] error)
{
	CheckAPIError("Pass", "UpdatePointCallback", response, error);
}

void UsePointBox(int client, bool all = false)
{
	static char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
		return;

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);
	dp.WriteCell(all, false);

	// JSON data
	JSONObject data = new JSONObject();
	data.SetInt64("uid", uid);
	data.SetBool("all", all);
	
	static char path[128];
	Format(path, sizeof(path), "%s/pointbox", GamePass_Service);
	NewNCSAPI(path).Post(data, UsePointBoxCallback, dp);
	delete data;
}

public void UsePointBoxCallback(HTTPResponse response, DataPack dp, const char[] error)
{
	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[MAX_UID_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	bool all = dp.ReadCell();
	dp.Close();

	if (CheckAPIError("Pass", "UsePointBoxCallback", response, error))
	{
		NCS_LogError("Pass", "UsePointBoxCallback", "#%d UID:%s All:%d", client, uid, all);

		if (IsValidClient(client))
			NCS_Chat(client, _, "使用通行证经验箱失败");
		return;
	}

	if (IsValidClient(client))
			NCS_Chat(client, _, "成功使用通行证经验箱");
}