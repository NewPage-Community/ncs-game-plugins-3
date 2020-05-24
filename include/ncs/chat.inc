#define CHAT_SYS_PREFIX "[系统提示]"
#define CLIENT_ALL 0

// Send a message with prefix (For plugin notification)
stock void NCS_Chat(int client = CLIENT_ALL, char[] prefix = CHAT_SYS_PREFIX, char[] msg, any ...)
{
	char buffer[512];
	VFormat(buffer, sizeof(buffer), msg, 4);
	Format(buffer, sizeof(buffer), "\x04%s\x01 %s", prefix, buffer);

	if (!client)
		CPrintToChatAll(buffer);
	else
		CPrintToChat(client, buffer);
}