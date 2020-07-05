#include <ncs/server>
#define Server_Service "user/ban"

HTTPClient httpClient;

void InitAPI()
{
	httpClient = NewNCSAPI();
}

void CloseAPI()
{
	delete httpClient;
}

void ReqBanInfo(int client, int uid)
{
	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteCell(uid, false);

	// JSON
	JSONObject data = new JSONObject();
	data.SetInt("uid", uid);
	data.SetInt("server_id", NCS_Server_GetID());
	data.SetInt("mod_id", NCS_Server_GetModID());
	data.SetInt("game_id", NCS_Server_GetGameID());

	char path[128];
	Format(path, sizeof(path), "%s/check/%d", Server_Service, uid);
	httpClient.Post(path, data, LoadBanInfo, dp);
}

public void LoadBanInfo(HTTPResponse response, DataPack dp, const char[] error)
{
	// Ban recode not found
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
	int uid = dp.ReadCell();
	dp.Close()

	// JSON
	char banReason[64], expireTime[20];
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONObject info = data.Get("info");

	// Check UID is valid
	if (info.GetInt("uid") != uid)
		return;

	// Get ban information
	info.GetString("reason", banReason, sizeof(banReason));
	info.GetString("expire_time", expireTime, sizeof(expireTime));
	int banType = info.GetInt("type");

	// Format expire time
	char time[64];
	FormatTime(time, sizeof(time), "%Y 年 %m 月 %d 日 %H:%M:%S", expireTime);

	// Ban client
	char kickReason[256];
	FormatEx(kickReason, sizeof(kickReason), "您已被服务器封锁,禁止进入游戏! 请联系管理员\n封禁类型: %s 封禁原因: %s\n到期时间: %s", g_banType[banType], banReason, time);
	BanClient(client, 10, BANFLAG_AUTHID, kickReason, kickReason);
	return;
}