#define REQUIRE_EXTENSIONS
#include <ripext>
#undef REQUIRE_EXTENSIONS

#define API_URL "https://api.new-page.xyz/ncs/v3"

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