#define MAX_LENGTH_FLAG		32
#define MAX_LENGTH_MESSAGE	128
#define MAX_LENGTH_BUFFER	255

static bool proto;
static bool newMSG[MAXPLAYERS + 1];
static EngineVersion engine;
static bool allowDeadtalk;
static ConVar cvarAllowDeadtalk;

void ChatConfig()
{
    engine = GetEngineVersion();
    proto = CanTestFeatures() && GetFeatureStatus(FeatureType_Native, "GetUserMessageType") == FeatureStatus_Available && GetUserMessageType() == UM_Protobuf;

    UserMsg SayText2 = GetUserMessageId("SayText2");

    if (SayText2 != INVALID_MESSAGE_ID)
        HookUserMessage(SayText2, OnSayText2, true);
    else
        SetFailState("Error loading the plugin, SayText2 is unavailable.");

    cvarAllowDeadtalk = FindConVar("sv_deadtalk");
    if (cvarAllowDeadtalk != INVALID_HANDLE)
    {
        allowDeadtalk = cvarAllowDeadtalk.BoolValue;
        cvarAllowDeadtalk.AddChangeHook(Deadtalk_Changed);
    }
    else
        allowDeadtalk = true;
}

//This function is based on chat-processor by Keith Warren 
public Action OnSayText2(UserMsg msg_id, BfRead msg, const int[] players, int playersNum, bool reliable, bool init)
{
    char game[32];
    GetGameFolderName(game, 32);

    //Retrieve the client sending the message to other clients.
    int iSender = proto ? PbReadInt(msg, "ent_idx") : BfReadByte(msg);

    if (iSender <= 0)
        return Plugin_Continue;

    //Stops double messages in-general.
    if (newMSG[iSender])
        newMSG[iSender] = false;
    else if (reliable)	//Fix for other plugins that use SayText2 I guess?
        return Plugin_Stop;

    //Chat Type
    bool bChat = proto ? PbReadBool(msg, "chat") : view_as<bool>(BfReadByte(msg));

    //Retrieve the name of template name to use when getting the format.
    char sFlag[MAX_LENGTH_FLAG];
    switch (proto)
    {
        case true: PbReadString(msg, "msg_name", sFlag, sizeof(sFlag));
        case false: BfReadString(msg, sFlag, sizeof(sFlag));
    }

    //Get the name string of the client.
    char sName[MAX_NAME_LENGTH];
    switch (proto)
    {
        case true: PbReadString(msg, "params", sName, sizeof(sName), 0);
        case false: if (BfGetNumBytesLeft(msg)) BfReadString(msg, sName, sizeof(sName));
    }

    //Get the message string that the client is wanting to send.
    char sMessage[MAX_LENGTH_MESSAGE];
    switch (proto)
    {
        case true: PbReadString(msg, "params", sMessage, sizeof(sMessage), 1);
        case false: if (BfGetNumBytesLeft(msg)) BfReadString(msg, sMessage, sizeof(sMessage));
    }

    //Get tag name.
    if (player[iSender].IsValid())
        player[iSender].GetChatName(sName, sizeof(sName));

    DataPack hPack = new DataPack();
    hPack.WriteCell(iSender);
    hPack.WriteString(sName);
    hPack.WriteString(sMessage);
    hPack.WriteString(sFlag);
    hPack.WriteCell(bChat);

    RequestFrame(Frame_OnChatMessage_SayText2, hPack);

    return Plugin_Stop;
}

public void Frame_OnChatMessage_SayText2(DataPack data)
{
    //Retrieve pack contents and what not, this part is obvious.
    data.Reset();

    int iSender = data.ReadCell();

    char sName[MAX_NAME_LENGTH];
    data.ReadString(sName, sizeof(sName));

    char sMessage[MAX_LENGTH_MESSAGE];
    data.ReadString(sMessage, sizeof(sMessage));

    char sFlag[MAX_LENGTH_FLAG];
    data.ReadString(sFlag, sizeof(sFlag));

    bool bChat = data.ReadCell();

    delete data;

    // All Server Chat
    //if (sMessage[0] == '#')
    //{
    //	NP_AllChat_Msg(PlayerChat, sName, sMessage);
    //	return;
    //}
    
    int team = GetClientTeam(iSender);
    bool alive = IsPlayerAlive(iSender);

    //Process colors.
    ProcessColorString(sName, MAX_NAME_LENGTH, GetClientTeam(iSender));

    char sBuffer[MAX_LENGTH_BUFFER];
    Format(sBuffer, MAX_LENGTH_BUFFER, "\x05%s\x01 : %s", sName, sMessage);

    if (team == 1)
        Format(sBuffer, MAX_LENGTH_BUFFER, "\x05*观察*\x01 %s", sBuffer);
    else if (!alive)
        Format(sBuffer, MAX_LENGTH_BUFFER, "\x05*死亡*\x01 %s", sBuffer);

    // CSGO quirk where the 1st color in the line won't work..
    if (engine == Engine_CSGO)
        Format(sBuffer, MAX_LENGTH_BUFFER, " %s", sBuffer);


    // Send the message to clients.
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i))
        {
            if (StrContains(sFlag, "_All") == -1 && team != GetClientTeam(i))
                continue;

            if (!allowDeadtalk && IsPlayerAlive(i) && !IsPlayerAlive(iSender))
                continue;

            if (proto)
                CSayText2(i, sBuffer, iSender, bChat);
            else
                SendPlayerMessage(i, sBuffer, iSender);
        }
    }
}

public Action OnClientSayCommand(int client, const char[] command, const char[] sArgs)
{
    if (client > 0 && StrContains(command, "say") != -1)
        newMSG[client] = true;
}

void CSayText2(int iClient, char[] sMessage, int iAuthor, bool bChat = true)
{
    Handle hMsg = StartMessageOne("SayText2", iClient, USERMSG_RELIABLE|USERMSG_BLOCKHOOKS);
    if(GetFeatureStatus(FeatureType_Native, "GetUserMessageType") == FeatureStatus_Available && GetUserMessageType() == UM_Protobuf)
    {
        PbSetInt(hMsg, "ent_idx", iAuthor);
        PbSetBool(hMsg, "chat", bChat);
        PbSetString(hMsg, "msg_name", sMessage);
        PbAddString(hMsg, "params", "");
        PbAddString(hMsg, "params", "");
        PbAddString(hMsg, "params", "");
        PbAddString(hMsg, "params", "");
    }
    else
    {
        BfWriteByte(hMsg, iAuthor);
        BfWriteByte(hMsg, true);
        BfWriteString(hMsg, sMessage);
    }
    EndMessage();
}

void SendPlayerMessage(int iClient, char[] sMessage, int iAuthor  = 0)
{
    if (iAuthor < 1 || iAuthor > MaxClients || !IsClientInGame(iAuthor))
    {
        PrintToChat(iClient, sMessage);
        if (iAuthor != 0)
            LogError("Client %d is not valid or in game", iAuthor);
    }
    else
        CSayText2(iClient, sMessage, iAuthor);
}

public void Deadtalk_Changed(ConVar convar, const char[] oldValue, const char[] newValue)
{
    allowDeadtalk = convar.BoolValue;
}