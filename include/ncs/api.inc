#define REQUIRE_EXTENSIONS
#include <ripext>
#undef REQUIRE_EXTENSIONS

#if defined DEV
	#define API_URL "http://127.0.0.1:8080"
#else
    #define API_URL "https://api.new-page.xyz/ncs/v3"
#endif

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

    return c
}

public void BlankCallback(HTTPResponse response, any value, const char[] error)
{
    if (response.Status != HTTPStatus_OK)
		NCS_LogError("API", "BlankCallback", "Faild to request %d (%s)", response.Status, error);
}