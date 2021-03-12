#define REQUIRE_EXTENSIONS
#include <ripext>
#undef REQUIRE_EXTENSIONS

#define API_URL "<api_url>"
#define TOKEN   "<api_token>"

stock HTTPClient NewNCSAPI()
{
    HTTPClient c = new HTTPClient(API_URL);

    char auth[64];
    Format(auth, sizeof(auth), "Basic %s", TOKEN);
    c.SetHeader("Authorization", auth);
    //c.SetHeader("X-B3-Sampled", "1");

    return c;
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