#define Chat_Service "game/chat"

HTTPClient httpClient;

void InitAPI()
{
    httpClient = NewNCSAPI();
}

void CloseAPI()
{
    delete httpClient;
}

void AllChat(const char[] name, const char[] message)
{
    // JSON
    JSONObject data = new JSONObject();
    data.SetString("name", name);
    data.SetString("message", message);

    char path[128];
    Format(path, sizeof(path), "%s/allchat", Chat_Service);
    httpClient.Post(path, data, BlankCallback);
}
