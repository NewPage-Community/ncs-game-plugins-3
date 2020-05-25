#define REQUIRE_EXTENSIONS
#include <ripext>
#undef REQUIRE_EXTENSIONS

#if defined DEV
	#define API_URL "http://127.0.0.1:8080"
#else
    #define API_URL "https://api.new-page.xyz/ncs3"
#endif


// NEWPAGE:API
#define TOKEN "TkVXUEFHRTpBUEk="

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