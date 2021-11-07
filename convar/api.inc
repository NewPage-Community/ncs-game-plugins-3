#define CVar_Service "game/cvar"



void InitAPI()
{
    InitNCSAPI();
}

void CloseAPI()
{
    CloseNCSAPI();
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
    GetNCSAPI().Post(path, data, LoadCVars);
}

public void LoadCVars(HTTPResponse response, any v, const char[] error)
{
    if (CheckAPIError("CVar", "LoadCVars", response, error))
        return;

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
