#include <ncs/server>
#define Ban_Service "user/ban"

HTTPClient httpClient;

void InitAPI()
{
	httpClient = NewNCSAPI();
}

void CloseAPI()
{
	delete httpClient;
}

void ReqBanCheck(int client, const char[] uid)
{
	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);

	// JSON
	JSONObject data = new JSONObject();
	data.SetInt("server_id", NCS_Server_GetID());
	data.SetInt("mod_id", NCS_Server_GetModID());
	data.SetInt("game_id", NCS_Server_GetGameID());

	char path[128];
	Format(path, sizeof(path), "%s/check/%s", Ban_Service, uid);
	httpClient.Post(path, data, LoadBanCheck, dp);
}

public void LoadBanCheck(HTTPResponse response, DataPack dp, const char[] error)
{
	// Ban record not found
	if (response.Status == HTTPStatus_NotFound)
		return;

	if (response.Status != HTTPStatus_OK || response.Data == null)
	{
		NCS_LogError("Bans", "LoadBanInfo", "Faild to load info %d (%s)", response.Status, error);
		return;
	}

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[INT64_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	dp.Close()

	// JSON
	char banReason[64], expireTime[20], iUID[INT64_LENGTH];
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONObject info = view_as<JSONObject>(data.Get("info"));

	// Check UID is valid
	info.GetString("uid", iUID, sizeof(iUID));
	if (!StrEqual(iUID, uid))
		return;

	// Get ban information
	info.GetString("reason", banReason, sizeof(banReason));
	info.GetString("expire_time", expireTime, sizeof(expireTime));
	int banType = info.GetInt("type");

	// Format expire time
	char time[64];
	FormatTime(time, sizeof(time), "%Y/%m/%d %H:%M:%S", StringToInt(expireTime));

	// Ban client
	char kickReason[256];
	FormatEx(kickReason, sizeof(kickReason), "您已被服务器封锁,禁止进入游戏! 请联系管理员\n封禁类型: %s 封禁原因: %s\n到期时间: %s", g_banType[banType], banReason, time);
	BanClient(client, 10, BANFLAG_AUTHID, kickReason, kickReason);
	return;
}

void ReqBanClient(int client, int type, int length, const char[] reason)
{
	char uid[MAX_UID_LENGTH];
	NCS_Account_GetUID(client, uid, sizeof(uid));
	if (uid[0] == '0')
		return;
	
	int iExpireTime[2], iLength[2];
	iLength[0] = length;
	GetTime(iExpireTime);
	Int64Sum(iExpireTime, iLength);

	char sExpireTime[20];
	Format(sExpireTime, sizeof(sExpireTime), "%d%d", iExpireTime[1], iExpireTime[0]);
	
	// JSON
	JSONObject info = new JSONObject();
	info.SetInt64("uid", uid);
	info.SetInt64("expire_time", sExpireTime);
	info.SetInt("type", type);
	info.SetInt("server_id", NCS_Server_GetID());
	info.SetInt("mod_id", NCS_Server_GetModID());
	info.SetInt("game_id", NCS_Server_GetGameID());
	info.SetString("reason", reason);

	JSONObject data = new JSONObject();
	data.Set("info", info);
	delete info;

	char path[128];
	Format(path, sizeof(path), "%s/add", Ban_Service);
	httpClient.Post(path, data, BlankCallback);
}