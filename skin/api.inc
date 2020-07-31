#define Skin_Service "game/skin"

HTTPClient httpClient;

void InitAPI()
{
    httpClient = NewNCSAPI();
}

void CloseAPI()
{
    delete httpClient;
}

void LoadSkins()
{
    char path[128];
    Format(path, sizeof(path), "%s", Skin_Service);
    httpClient.Get(path, LoadSkinsInfo);
}

public void LoadSkinsInfo(HTTPResponse response, DataPack dp, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Skin", "LoadSkinsInfo", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

    // JSON
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray skins = view_as<JSONArray>(data.Get("info"));
    int length = skins.Length;

    // Loading skins
    if (g_Skins != INVALID_HANDLE)
        delete g_Skins;
    g_Skins = new SkinIndex();

    for (int i = 0; i < length; i++)
        g_Skins.Add(view_as<JSONObject>(skins.Get(i)));
    
    delete skins;
}

void ReqUserSkinInfo(int client, const char[] uid)
{
    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteString(uid, false);

    char path[128];
    Format(path, sizeof(path), "%s/%s", Skin_Service, uid);
    httpClient.Get(path, LoadUserSkinInfo, dp);
}

public void LoadUserSkinInfo(HTTPResponse response, DataPack dp, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("Skin", "LoadUserSkinInfo", "Faild to load info %d (%s)", response.Status, error);
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

    // Check UID is valid
    data.GetString("uid", iUID, sizeof(iUID));
    if (!StrEqual(iUID, uid))
        return;

    g_UsersUsedSkin[client] = data.GetInt("used_skin");
}

void SetUsedSkin(int client, int id)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
    {
        NCS_Chat(client, _, "{blue}您的用户数据尚未加载，请重试！");
        return;
    }

    g_UsersUsedSkin[client] = id;

    // JSON data
    JSONObject data = new JSONObject();
    data.SetInt("used_skin", id);

    char path[128];
    Format(path, sizeof(path), "%s/%s/used", Skin_Service, uid);
    httpClient.Post(path, data, BlankCallback);
}