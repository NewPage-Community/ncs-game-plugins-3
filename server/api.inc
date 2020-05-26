#define Server_Service "server/info"

HTTPClient httpClient;

GlobalForward onLoaded;

methodmap ServerInfo < JSONObject
{
	// Constructor
	public ServerInfo()
	{
		JSONObject s = new JSONObject();

		s.SetInt("server_id", 0);
		s.SetInt("mod_id", 0);
		s.SetInt("game_id", 0);
		s.SetString("rcon", "RCONPASSWORD");
		s.SetString("hostname", "NewPage Server");

		// IP & Port
		ConVar hostip = FindConVar("hostip"), hostport = FindConVar("hostport");
		int ip = hostip.IntValue;
		char c_ip[24];
		FormatEx(c_ip, 24, "%d.%d.%d.%d", ((ip & 0xFF000000) >> 24) & 0xFF, ((ip & 0x00FF0000) >> 16) & 0xFF, ((ip & 0x0000FF00) >>  8) & 0xFF, ((ip & 0x000000FF) >>  0) & 0xFF);
		s.SetString("ip", c_ip);
		s.SetInt("port", hostport.IntValue);
		delete hostip;
		delete hostport;

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
	httpClient = NewNCSAPI();
}

void RegForward()
{
	onLoaded = new GlobalForward("NCS_Server_OnLoaded",  ET_Ignore);
}

void ReqServerInfo()
{
	httpClient.Post(Server_Service, new ServerInfo(), LoadServerInfo);
}

public void LoadServerInfo(HTTPResponse response, any value, const char[] error)
{
	bool Loaded = false;
	ServerInfo info;

	if (response.Status != HTTPStatus_OK || response.Data == null)
	{
		info = new ServerInfo()
		PrintToServer("[NCS] Server - Faild to load info %d (%s)", response.Status, error);
	}
	else
	{
		info = view_as<ServerInfo>(response.Data);
		Loaded = true;
	}

	// Load info data
	ServerID = info.ServerID;
	ModID = info.ModID;
	GameID = info.GameID;
	// Set RCON password
	info.SetRcon()
	// Set Hostname
	info.SetHostname();

	if (Loaded)
		PrintToServer("[NCS] Server - Loaded!");

	// Call global forward
	Call_StartForward(onLoaded);
	Call_Finish();
}