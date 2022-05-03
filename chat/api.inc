#define Chat_Service "game/chat"

void InitAPI()
{
    InitNCSAPI();
}

void CloseAPI()
{

}

void AllChat(int client, const char[] name, const char[] message)
{
    static char uid[MAX_UID_LENGTH];
    if (!NCS_Account_GetUID(client, uid, MAX_UID_LENGTH))
        return;

    // JSON
    JSONObject data = new JSONObject();
    data.SetString("name", name);
    data.SetString("message", message);
    data.SetInt("server_id", NCS_Server_GetID());
    data.SetInt64("uid", uid);

    static char path[128];
    Format(path, sizeof(path), "%s/allchat", Chat_Service);
    NewNCSAPI(path).Post(data, AllChatCallback);
	delete data;
}

public void AllChatCallback(HTTPResponse response, any value, const char[] error)
{
    CheckAPIError("Chat", "AllChatCallback", response, error);
}