#define REQUIRE_EXTENSIONS
#include <ripext>
#undef REQUIRE_EXTENSIONS

#define CONVARAPIURL "ncs_api_url"
#define CONVARAPITOKEN "ncs_api_token"

static ConVar url;
static ConVar token;

stock void InitNCSAPI()
{
    // ConVar init
    if ((url = FindConVar(CONVARAPIURL)) == null)
        url = CreateConVar(CONVARAPIURL, "", "");
    if ((token = FindConVar(CONVARAPITOKEN)) == null)
        token = CreateConVar(CONVARAPITOKEN, "", "");
}

stock HTTPRequest NewNCSAPI(const char[] path)
{
    static char buffer[128];

    // New Client
    url.GetString(buffer, sizeof(buffer));
    Format(buffer, sizeof(buffer), "%s/%s", buffer, path);
    HTTPRequest client = new HTTPRequest(buffer);

    // Http auth
    token.GetString(buffer, sizeof(buffer));
    Format(buffer, sizeof(buffer), "Basic %s", buffer);
    client.SetHeader("Authorization", buffer);

	client.ConnectTimeout = 3;
	client.Timeout = 5;

    // tracing
    //c.SetHeader("X-B3-Sampled", "1");
    return client;
}

// Check has error ?
stock bool CheckAPIError(const char[] module, const char[] func, HTTPResponse response, const char[] error)
{
    if (response.Status == HTTPStatus_OK)
        return false;

    NCS_LogError(module, func, "Faild to request HTTPCode: %d Err: %s", response.Status, error);
    if (response.Status != HTTPStatus_Invalid)
    {
        char json[512];
        response.Data.ToString(json, sizeof(json));
        NCS_LogError(module, func, "Response JSON: %s", json);
    }
    return true;
}