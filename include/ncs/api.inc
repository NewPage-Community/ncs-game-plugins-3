#define REQUIRE_EXTENSIONS
#include <ripext>
#undef REQUIRE_EXTENSIONS

#define API_URL "https://api.new-page.xyz/ncs3"

// NEWPAGECORESYSTEMAPI
#define TOKEN "TkVXUEFHRUNPUkVTWVNURU1BUEk="

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