#define BackpackItems_Service "backpack/items"
#define BackpackUser_Service "backpack/user"



void InitAPI()
{
    InitNCSAPI();
}

void CloseAPI()
{

}

void LoadSkins()
{
    char path[128];
    Format(path, sizeof(path), "%s/type/1", BackpackItems_Service);
    NewNCSAPI(path).Get(LoadSkinsInfo);
}

public void LoadSkinsInfo(HTTPResponse response, DataPack dp, const char[] error)
{
    if (CheckAPIError("Skin", "LoadSkinsInfo", response, error))
        return;

    // JSON
    JSONObject data = view_as<JSONObject>(response.Data);
    JSONArray skins = view_as<JSONArray>(data.Get("items"));
    int length = skins.Length;

    // Loading skins
    if (g_Skins != INVALID_HANDLE)
        delete g_Skins;
    g_Skins = new SkinIndex();

    for (int i = 0; i < length; i++)
    {
        JSONObject skin = view_as<JSONObject>(skins.Get(i));
        g_Skins.Add(skin);
        delete skin;
    }
    
    delete skins;
}

void CheckUserHaveSkin(int client)
{
    char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, sizeof(uid)))
        return;

    int id = g_UsersUsedSkin[client];
    
    // Datapack to pass to callback
    DataPack dp = new DataPack();
    dp.WriteCell(client, false);
    dp.WriteCell(id, false);
    
    char path[128];
    Format(path, sizeof(path), "%s/have/%s/%d", BackpackUser_Service, uid, id);
    NewNCSAPI(path).Get(CheckUserHaveItemCallback, dp);
}

public void CheckUserHaveItemCallback(HTTPResponse response, DataPack dp, const char[] error)
{
    if (CheckAPIError("Skin", "CheckUserHaveItemCallback", response, error))
        return;

    // DataPack
    dp.Reset();
    int client = dp.ReadCell();
    int id = dp.ReadCell();
    dp.Close();

    if (!IsValidClient(client))
        return;
    
     // JSON
    JSONObject data = view_as<JSONObject>(response.Data);
    bool have = data.GetBool("have");

    if (g_UsersUsedSkin[client] == id && !have)
        g_UsersUsedSkin[client] = 0;
}