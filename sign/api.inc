#include <ncs/server>
#define Sign_Service "user/sign"

HTTPClient httpClient;

GlobalForward onUserSigned;

void InitAPI()
{
	httpClient = NewNCSAPI();
	RegAPIForward();
}

void RegAPIForward()
{
	onUserSigned = new GlobalForward("NCS_Sign_OnUserSigned",  ET_Ignore, Param_Cell);
}

void CloseAPI()
{
	delete httpClient;
	CloseAPIForward();
}

void CloseAPIForward()
{
	delete onUserSigned;
}

void Sign(int client)
{
	char uid[MAX_UID_LENGTH];
	if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
	{
		NCS_Chat(client, _, "{blue}您的用户数据尚未加载，请重试！");
		return;
	}

	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);

	// JSON data
	JSONObject data = new JSONObject();
	data.SetInt64("uid", uid);

	char path[128];
	Format(path, sizeof(path), "%s", Sign_Service);
	httpClient.Post(path, data, SignCallback, dp);
}

public void SignCallback(HTTPResponse response, DataPack dp, const char[] error)
{
	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[INT64_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	dp.Close()

	if (!IsValidClient(client))
		return;

	if (response.Status != HTTPStatus_OK || response.Data == null)
	{
		NCS_LogError("Sign", "SignCallback", "Faild to load info %d (%s)", response.Status, error);
		NCS_LogError("Sign", "SignCallback", "#%d UID:%s", client, uid);
		NCS_Chat(client, _, "{blue}签到失败，请重试！");
		return;
	}

	JSONObject data = view_as<JSONObject>(response.Data);
	bool alreadySigned = data.GetBool("already_signed");

	if (alreadySigned)
	{
		NCS_Chat(client, _, "{blue}您今日已签到，请勿重复操作！");
		return;
	}

	NCS_Chat(client, _, "{blue}签到成功！");

	// Call global forward
	Call_StartForward(onUserSigned);
	Call_PushCell(client);
	Call_Finish();
}