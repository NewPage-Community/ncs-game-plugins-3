#define  Title_Service "user/title"
#include <ncs/account>
#include <clients>
HTTPClient httpClient;

enum struct TitleData
{
	char title[MAX_NAME_LENGTH];
	int type;
}

TitleData customTitle[];


void InitAPI()
{
	httpClient = NewNCSAPI();
}

void CloseAPI()
{
	delete httpClient;
}

void RequreTitle(int client, const char[] uid){
	if (!IsValidClient(client))
		return ;


	// Datapack to pass to callback
	DataPack dp = new DataPack();

	dp.WriteCell(client, false);
	dp.WriteString(uid, false);

	char path[128];
	Format(path, sizeof(path), "%s/%s", Title_Service, uid);
	httpClient.Get(path, RequreTitle_Resp, dp);

}

void RequreTitle_Resp(HTTPResponse response, DataPack dp, const char[] error){

	if (response.Status != HTTPStatus_OK || response.Data == null)
	{
		NCS_LogError("Title", "LoadTitleInfo", "Faild to load info %d (%s)", response.Status, error);
		return;
	}

	// DataPack
	dp.Reset();
	int client = dp.ReadCell();
	char uid[INT64_LENGTH];
	// char customTitle[MAX_NAME_LENGTH];
	dp.ReadString(uid, sizeof(uid));
	dp.Close();

	// JSON
	char iUID[INT64_LENGTH];
	JSONObject data = view_as<JSONObject>(response.Data);
	JSONObject info = view_as<JSONObject>(data.Get("info"));

	// Check UID is valid
	info.GetString("uid", iUID, sizeof(iUID));
	if (!StrEqual(iUID, uid)){
		return;
	}

	char title[MAX_NAME_LENGTH] ;
	
	info.GetString("custom_title", title, sizeof(title));
	customTitle[client].type = info.GetInt("title_type");

	customTitle[client].title = title;

	
	TitleBeSet(client, customTitle[client].type);
}

void SetTitle(int client , char[] newtitle){
	if (!IsValidClient(client))
		return ;


	char uid[MAX_UID_LENGTH];
	char path[128];
	
	NCS_Account_GetUID(client, uid, sizeof(uid));

	JSONObject data = new JSONObject();
	JSONObject info = new JSONObject();
	data.SetInt("uid", StringToInt(uid));
	data.SetString("custom_title", newtitle);
	data.SetInt("title_type",1);

	info.Set("info", data);

	Format(path, sizeof(path), "%s/%s", Title_Service, uid);
	httpClient.Put(path, info, BlankCallback);

}