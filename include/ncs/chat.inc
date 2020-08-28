#if defined _NCS_Chat_
    #endinput
#endif
#define _NCS_Chat_

#include <ncs/color>

#define MAX_NAME_LENGTH_FIX 256
#define MAX_TITLE_LENGTH 32
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

native void NCS_Chat_SetTitle(int client, const char[] title, const char[] titleColor);
native void NCS_Chat_SetNameColor(int client, const char[] nameColor);
native void NCS_Chat_GetChatName(int client, char[] buffer, int size);

public SharedPlugin __pl_ncs_chat = 
{
    name = "NCS-Chat",
    file = "np-chat.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_chat_SetNTVOptional()
{
    MarkNativeAsOptional("NCS_Chat_SetTitle");
    MarkNativeAsOptional("NCS_Chat_SetNameColor");
    MarkNativeAsOptional("NCS_Chat_GetChatName");
}
#endif