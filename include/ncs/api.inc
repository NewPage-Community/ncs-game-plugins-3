#define REQUIRE_EXTENSIONS
#include <ripext>
#undef REQUIRE_EXTENSIONS

#define API_URL "https://api.new-page.xyz/ncs/v3/game"

#if defined DEV
	#define TOKEN "TkVXUEFHRTpBUEk="
#else
    #define TOKEN "<api_token>"
#endif

stock HTTPClient NewNCSAPI()
{
    HTTPClient c = new HTTPClient(API_URL);

    c.ConnectTimeout = 1;
    c.Timeout = 5;
    c.FollowLocation = false;

    char auth[64];
    Format(auth, sizeof(auth), "Basic %s", TOKEN);
    c.SetHeader("Authorization", auth);
    c.SetHeader("X-B3-Sampled", "1");

    return c;
}

public void BlankCallback(HTTPResponse response, any value, const char[] error)
{
    if (response.Status != HTTPStatus_OK)
		NCS_LogError("API", "BlankCallback", "Faild to request %d (%s)", response.Status, error);
}

stock bool CheckAPIError(const char[] module, const char[] func, HTTPResponse response, const char[] error)
{
    if (response.Status == HTTPStatus_OK && response.Data != null)
        return false;

    if (response.Data != null)
    {
        char json[512];
        response.Data.ToString(json, sizeof(json));
        NCS_LogError(module, func, "JSON: %s", json);
    }
    NCS_LogError(module, func, "Faild to request %d (%s)", response.Status, error);
    return true;
}