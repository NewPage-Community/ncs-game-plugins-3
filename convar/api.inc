#define CVar_Service "game/cvar"

HTTPClient httpClient;

void InitAPI()
{
    httpClient = NewNCSAPI();
}

void CloseAPI()
{
    delete httpClient;
}

void UpdateCVars()
{
    // JSON data
    JSONObject data = new JSONObject();
    data.SetInt("game_id", NCS_Server_GetGameID());
    data.SetInt("mod_id", NCS_Server_GetModID());
    data.SetInt("server_id", NCS_Server_GetID());

    char path[128];
    Format(path, sizeof(path), "%s/update", CVar_Service);
    httpClient.Post(path, data, LoadCVars);
}

public void LoadCVars(HTTPResponse response, any v, const char[] error)
{
    if (response.Status != HTTPStatus_OK || response.Data == null)
    {
        NCS_LogError("CVar", "LoadCVars", "Faild to load info %d (%s)", response.Status, error);
        return;
    }

    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray cvars = view_as<JSONArray>(data.Get("cvars"));

    int length = cvars.Length;
    for (int i; i < length; i++)
    {
        char key[128], value[128];
        JSONObject cvar = view_as<JSONObject>(cvars.Get(i));
        cvar.GetString("key", key, sizeof(key));
        cvar.GetString("value", value, sizeof(value));
        delete cvar;

        ConVar c = FindConVar(key);
        if (c != INVALID_HANDLE)
        {
            c.SetString(value, false, false);
            delete c;
        }
    }
    delete cvars;

    PrintToServer("[NCS] ConVar - Updated!");
}
