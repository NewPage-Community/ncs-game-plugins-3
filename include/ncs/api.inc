#define REQUIRE_EXTENSIONS
#include <ripext>
#undef REQUIRE_EXTENSIONS

#define CONVARAPIURL "ncs_api_url"
#define CONVARAPITOKEN "ncs_api_token"

static HTTPClient gHTTPClient;
static ConVar url;
static ConVar token;

stock void InitNCSAPI()
{
    char buffer[128];

    // ConVar init
    if ((url = FindConVar(CONVARAPIURL)) == null)
        url = CreateConVar(CONVARAPIURL, "", "");
    if ((token = FindConVar(CONVARAPITOKEN)) == null)
        token = CreateConVar(CONVARAPITOKEN, "", "");
    // ConVar change hook
    url.AddChangeHook(NCSAPIChanged);
    token.AddChangeHook(NCSAPIChanged);

    AutoExecConfig(true, "ncs");

    // New Client
    url.GetString(buffer, sizeof(buffer));
    gHTTPClient = new HTTPClient(buffer);

    // Http auth
    token.GetString(buffer, sizeof(buffer));
    Format(buffer, sizeof(buffer), "Basic %s", buffer);
    gHTTPClient.SetHeader("Authorization", buffer);

    // tracing
    //c.SetHeader("X-B3-Sampled", "1");
}

public void NCSAPIChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
    CloseNCSAPI();
    InitNCSAPI();
}

stock HTTPClient GetNCSAPI()
{
    return gHTTPClient;
}

stock void CloseNCSAPI()
{
    delete gHTTPClient;
}

// Check has error ?
stock bool CheckAPIError(const char[] module, const char[] func, HTTPResponse response, const char[] error)
{
    if (response.Status == HTTPStatus_OK)
        return false;

    if (response.Status != HTTPStatus_Invalid)
    {
        char json[512];
        response.Data.ToString(json, sizeof(json));
        NCS_LogError(module, func, "Response JSON: %s", json);
    }
    NCS_LogError(module, func, "Faild to request HTTPCode: %d Err: %s", response.Status, error);
    return true;
}