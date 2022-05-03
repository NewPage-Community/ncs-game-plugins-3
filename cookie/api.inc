#define Cookie_Service "game/cookie"



static GlobalForward onUserCached;

void InitAPI()
{
    InitNCSAPI();
    onUserCached = new GlobalForward("NCS_Cookie_OnUserCached",  ET_Ignore, Param_Cell);
}

void CloseAPI()
{

}

void LoadUserCookie(int client, const char[] uid)
{
    player[client].Clear();
    
    static char path[128];
    Format(path, sizeof(path), "%s/all/%s", Cookie_Service, uid);
    NewNCSAPI(path).Get(LoadUserCookieCallback, client);
}

public void LoadUserCookieCallback(HTTPResponse response, int client, const char[] error)
{
    if (CheckAPIError("Cookie", "LoadUserCookieCallback", response, error))
        return;

    if (!IsValidClient(client))
        return;

    JSONObject data = view_as<JSONObject>(response.Data);
    JSONObject cookie = view_as<JSONObject>(data.Get("cookie"));

    // Init
    player[client].Init(client, cookie);

    delete cookie;

    // Call global forward
    Call_StartForward(onUserCached);
    Call_PushCell(client);
    Call_Finish();
}

bool SetUserCookie(int client, const char[] key, const char[] value)
{
    static char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return false;

    bool ok = player[client].Set(key, value);

    // JSON data
    JSONObject data = new JSONObject();
    data.SetInt64("uid", uid);
    data.SetString("key", key);
    data.SetString("value", value);

    static char path[128];
    Format(path, sizeof(path), "%s/set", Cookie_Service);
    NewNCSAPI(path).Post(data, SetUserCookieCallback);
	delete data;
    return ok;
}

public void SetUserCookieCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Cookie", "SetUserCookieCallback", response, error);
}