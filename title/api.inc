#define  Title_Service "user/title"
#include <ncs/account>
HTTPClient httpClient;

void InitAPI()
{
	httpClient = NewNCSAPI();
}

void CloseAPI()
{
	delete httpClient;
}

void RequreTitle(int client, const char[] uid){
	// Datapack to pass to callback
	DataPack dp = new DataPack();
	dp.WriteCell(client, false);
	dp.WriteString(uid, false);

	char path[128];
	Format(path, sizeof(path), "%s/%s", Title_Service, uid);
	httpClient.Get(path, RequreTitle_Resp, dp);
}

void RequreTitle_Resp(HTTPResponse response, DataPack dp, const char[] error){

	if (response.Status == HTTPStatus_NotFound)
		return;

	if (response.Status != HTTPStatus_OK || response.Data == null)
	{
		NCS_LogError("Title", "LoadTitleInfo", "Faild to load info %d (%s)", response.Status, error);
		return;
	}

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[INT64_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	dp.Close();

	// JSON
	char iUID[INT64_LENGTH];
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONObject info = view_as<JSONObject>(data.Get("info"));

	// Check UID is valid
	info.GetString("uid", iUID, sizeof(iUID));
	if (!StrEqual(iUID, uid))
		return;

	char customTitle = info.GetInt("custom_title");
	int customType = info.GetInt("title_type");

	Title_set(client, customType, customTitle);

}

void SetTitle(int client , char[] newtitle){
	char uid[MAX_UID_LENGTH];
	char path[128];
	
	NCS_Account_GetUID(client, uid, sizeof(uid));

	JSONObject info = new JSONObject();
	info.SetInt("uid", StringToInt(uid));
	info.SetString("custom_title", newtitle);
	info.SetInt("title_type",1);

	Format(path, sizeof(path), "%s/%s", Title_Service, uid);
	httpClient.Put(path, info, BlankCallback);

}