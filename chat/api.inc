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
    data.SetInt("server_id", NCS_Server_GetID());

    char path[128];
    Format(path, sizeof(path), "%s/allchat", Chat_Service);
    httpClient.Post(path, data, AllChatCallback);
}

public void AllChatCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Chat", "AllChatCallback", response, error);
}