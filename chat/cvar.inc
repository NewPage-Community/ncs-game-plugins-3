ConVar allowAllChat;

void InitConVar()
{
    allowAllChat = CreateConVar("chat_allow_allchat", "1", "", 0, true, 0.0, true, 1.0);
}