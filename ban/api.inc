#include <ncs/server>
#define Ban_Service "user/ban"

int gameAppID;

void InitAPI()
{
	InitNCSAPI();
	gameAppID = GetGameAppID();
}

void CloseAPI()
{
}

void ReqBanCheck(int client, const char[] uid)
{
	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);

	// JSON
	char ip[32]
	GetClientIP(client, ip, sizeof(ip));
	JSONObject data = new JSONObject();
	data.SetInt("server_id", NCS_Server_GetID());
	data.SetInt("mod_id", NCS_Server_GetModID());
	data.SetInt("game_id", NCS_Server_GetGameID());
	data.SetInt("app_id", gameAppID);
	data.SetString("ip", ip);

	static char path[128];
	Format(path, sizeof(path), "%s/check/%s", Ban_Service, uid);
	NewNCSAPI(path).Post(data, LoadBanCheck, dp);
	delete data;

	static char json[512];
	data.ToString(json, sizeof(json));
	NCS_LogMessage("Ban", "ReqBanCheck", "%L uid:%s request(%s)", client, uid, json);
}

public void LoadBanCheck(HTTPResponse response, DataPack dp, const char[] error)
{
	if (CheckAPIError("Bans", "LoadBanCheck", response, error))
		return;

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[MAX_UID_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	dp.Close();

	// JSON
	char banReason[64], expireTime[20], iUID[MAX_UID_LENGTH];
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONObject info = view_as<JSONObject>(data.Get("info"));
	// Get ban information
	info.GetString("uid", iUID, sizeof(iUID));
	info.GetString("reason", banReason, sizeof(banReason));
	info.GetString("expire_time", expireTime, sizeof(expireTime));
	int banType = info.GetInt("type");
	int banID = info.GetInt("id");
	bool blockIP = info.GetBool("block_ip");
	delete info;

	if (blockIP)
	{
		KickClient(client, "您的IP已被阻止进入游戏！");
		return;
	}

	// Check UID is valid
	// Check user banning info
	if (!StrEqual(iUID, uid) || !IsValidClient(client) || banID == 0)
		return;

	// Format expire time
	char time[256];
	FormatTime(time, sizeof(time), NULL_STRING, StringToInt(expireTime));

	// Ban client
	char kickReason[256];
	FormatEx(kickReason, sizeof(kickReason), "您已被服务器封锁,禁止进入游戏! 请联系管理员\n封禁类型: %s 封禁原因: %s\n到期时间: %s", g_banType[banType], banReason, time);
	BanClient(client, 10, BANFLAG_AUTHID, kickReason, kickReason);
}

void ReqBanClient(int client, int type, int length, const char[] reason)
{
	static char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
		return;
	
	int iExpireTime[2], iLength[2];
	iLength[0] = length;
	GetTime(iExpireTime);
	Int64Sum(iExpireTime, iLength);

	char sExpireTime[20];
	Format(sExpireTime, sizeof(sExpireTime), "%d%d", iExpireTime[1], iExpireTime[0]);

	char ip[32]
	GetClientIP(client, ip, sizeof(ip));
	
	// JSON
	JSONObject info = new JSONObject();
	info.SetInt64("uid", uid);
	info.SetInt64("expire_time", sExpireTime);
	info.SetInt("type", type);
	info.SetInt("server_id", NCS_Server_GetID());
	info.SetInt("mod_id", NCS_Server_GetModID());
	info.SetInt("game_id", NCS_Server_GetGameID());
	info.SetString("reason", reason);
	info.SetInt("app_id", gameAppID);
	info.SetString("ip", ip);

	JSONObject data = new JSONObject();
	data.Set("info", info);
	delete info;

	static char path[128];
	Format(path, sizeof(path), "%s/add", Ban_Service);
	NewNCSAPI(path).Post(data, ReqBanClientCallback);
	delete data;
}

public void ReqBanClientCallback(HTTPResponse response, any value, const char[] error)
{
	CheckAPIError("Ban", "ReqBanClientCallback", response, error);
}