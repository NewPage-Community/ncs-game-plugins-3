#define Server_Service "game/server"



GlobalForward onLoaded;

methodmap ServerInfo < JSONObject
{
    // Constructor
    public ServerInfo()
    {
        JSONObject s = new JSONObject();
        // Random rcon
        char random_rcon[32];
        GenerateRandomString(random_rcon, sizeof(random_rcon));

        s.SetInt("server_id", 0);
        s.SetInt("mod_id", 0);
        s.SetInt("game_id", 0);
        s.SetString("rcon", random_rcon);
        s.SetString("hostname", "NewPage Server");

        return view_as<ServerInfo>(s);
    }

    public void SetRcon()
    {
        ConVar rcon = FindConVar("rcon_password");
        char c_rcon[128];
        this.GetString("rcon", c_rcon, sizeof(c_rcon));
        rcon.SetString(c_rcon, false, false);
        delete rcon;
    }

    public void SetHostname()
    {
        ConVar hostname = FindConVar("hostname");
        char c_hostname[128];
        this.GetString("hostname", c_hostname, sizeof(c_hostname));
        hostname.SetString(c_hostname, false, false);
        delete hostname;
    }

    property int ServerID {
        public get() { return this.GetInt("server_id"); }
    }

    property int ModID {
        public get() { return this.GetInt("mod_id"); }
    }

    property int GameID {
        public get() { return this.GetInt("game_id"); }
    }
};

void InitAPI()
{
    InitNCSAPI();
}

void RegForward()
{
    onLoaded = new GlobalForward("NCS_Server_OnLoaded",  ET_Ignore);
}

void CloseAPI()
{

}

void ReqServerInfo()
{
    // IP & Port
    ConVar hostip = FindConVar("hostip"), hostport = FindConVar("hostport");
    int ip = hostip.IntValue;
    char c_ip[24];
    FormatEx(c_ip, 24, "%d.%d.%d.%d", ((ip & 0xFF000000) >> 24) & 0xFF, ((ip & 0x00FF0000) >> 16) & 0xFF, ((ip & 0x0000FF00) >>  8) & 0xFF, ((ip & 0x000000FF) >>  0) & 0xFF);
    
    // JSON
    JSONObject data = new JSONObject();
    data.SetString("address", c_ip);
    data.SetInt("port", hostport.IntValue);
    
    char path[64];
    Format(path, sizeof(path), "%s/init", Server_Service);
    NewNCSAPI(path).Post(data, LoadServerInfo);
    delete hostip;
    delete hostport;
}

public void LoadServerInfo(HTTPResponse response, any value, const char[] error)
{
    bool Loaded = false;
    ServerInfo info;

    if (CheckAPIError("Server", "LoadServerInfo", response, error))
    {
        info = new ServerInfo();
        CreateTimer(10.0, Timer_Reload, 0, TIMER_FLAG_NO_MAPCHANGE);
    }
    else
    {
        JSONObject data = view_as<JSONObject>(response.Data);
        info = view_as<ServerInfo>(data.Get("info"));
        Loaded = true;
    }

    // Load info data
    ServerID = info.ServerID;
    ModID = info.ModID;
    GameID = info.GameID;
    // Set RCON password
    info.SetRcon();
    // Set Hostname
    info.SetHostname();

    if (Loaded)
        PrintToServer("[NCS] Server - Loaded!");
    else
        PrintToServer("[NCS] Server - Failed to load!");

    // Call global forward
    Call_StartForward(onLoaded);
    Call_Finish();

    delete info;
}

public Action Timer_Reload(Handle timer)
{
    ReqServerInfo();
    return Plugin_Stop;
}

void ReqMapNotify(int serverID, const char[] map)
{
    // JSON
    JSONObject data = new JSONObject();
    data.SetInt("server_id", serverID);
    data.SetString("map", map);
    
    char path[64];
    Format(path, sizeof(path), "%s/changemap", Server_Service);
    NewNCSAPI(path).Post(data, ReqMapNotifyCallback);
}

public void ReqMapNotifyCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Server", "ReqMapNotifyCallback", response, error);
}