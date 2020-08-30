#pragma semicolon 1

#include <ncs/client>
#include <ncs/color>
#include <ncs/vip>
#include <ncs/chat>

#include <sdktools>

#define MAXCHEERS 32
#define MAX_CHEER_TYPE 2
#define MAX_CHAT_LENGTH 512

enum CheerType
{
    Cheer = 0,
    Jeer
}

char g_cCheerString[5][32];

char g_cCheerList[MAX_CHEER_TYPE][MAXCHEERS][PLATFORM_MAX_PATH];

int g_iCheerCount[MAXPLAYERS + 1][MAX_CHEER_TYPE],
    g_iCheerListNum[MAX_CHEER_TYPE] = 0;

ConVar  g_cClientMaxCheers,
        g_cClientMaxJeers,
        g_cVIPCheersAdd,
        g_cVIPJeersAdd,
        g_cSoundVolume;

public Plugin myinfo = 
{
    name        = "Cheer (NCS.ver)",
    author      = "Gunslinger",
    description = "Play cheer sound",
    version     = "1.1",
    url         = "https://blog.new-page.xyz"
};

public void OnPluginStart()
{
    g_cClientMaxCheers = CreateConVar("sm_cheer_limit", "3", "The maximum number of cheers per round");
    g_cClientMaxJeers = CreateConVar("sm_cheer_jeer_limit", "1", "The maximum number of jeers per round");
    g_cVIPCheersAdd = CreateConVar("sm_cheer_vip_add", "3", "Addition number of vip cheers per round");
    g_cVIPJeersAdd = CreateConVar("sm_cheer_jeer_vip_add", "3", "Addition number of vip jeers per round");
    g_cSoundVolume = CreateConVar("sm_cheer_volume", "1.0", "Cheer volume: should be a number between 0.0. and 1.0");
    
    // Execute the config file
    AutoExecConfig(true, "cheer");
    
    HookEvent("round_start", EventRoundStart, EventHookMode_PostNoCopy);
    RegConsoleCmd("cheer", CommandCheer);
    RegAdminCmd("sm_cheertest", CommandCheerTest, ADMFLAG_ROOT);

    g_cCheerString[0] = "捧腹大笑";
    g_cCheerString[1] = "笑而不语";
    g_cCheerString[2] = "哭笑不得";
    g_cCheerString[3] = "忍俊不禁";
    g_cCheerString[4] = "喜笑颜开";
}

public void OnMapStart()
{
    LoadSounds();
}

public void OnClientPutInServer(int client)
{
    if(client && !IsFakeClient(client))
    {
        g_iCheerCount[client][Cheer] = 0;
        g_iCheerCount[client][Jeer] = 0;
    }
}

public void EventRoundStart(Handle event, const char[] name, bool dontBroadcast)
{
    for (int i = 1; i <= MAXPLAYERS; i++)
    {
        g_iCheerCount[i][Cheer] = 0;
        g_iCheerCount[i][Jeer] = 0;
    }
}

void LoadSounds()
{
    g_iCheerListNum[Cheer] = 0;
    g_iCheerListNum[Jeer] = 0;

    KeyValues kv = new KeyValues("CheerSoundsList");
    char filename[PLATFORM_MAX_PATH], buffer[30], downloadFile[PLATFORM_MAX_PATH];

    BuildPath(Path_SM, filename, PLATFORM_MAX_PATH, "configs/cheersoundlist.cfg");
    kv.ImportFromFile(filename);
    
    if (!kv.JumpToKey("Cheer"))
    {
        SetFailState("configs/cheersoundlist.cfg missing cheer sounds");
        delete kv;
        return;
    }

    for (int i = 0; i < MAXCHEERS; i++)
    {
        Format(buffer, sizeof(buffer), "%i", i + 1);
        kv.GetString(buffer, g_cCheerList[Cheer][i], PLATFORM_MAX_PATH);
        if (g_cCheerList[Cheer][i][0] != '\0')
        {
            Format(downloadFile, PLATFORM_MAX_PATH, "sound/%s", g_cCheerList[Cheer][i]);
            if (FileExists(downloadFile, true))
            {
                PrecacheSound(g_cCheerList[Cheer][i], true);
                AddFileToDownloadsTable(downloadFile);
                g_iCheerListNum[Cheer]++;
            }
        }	
    }
    
    kv.Rewind();

    if (!kv.JumpToKey("Jeer"))
    {
        SetFailState("configs/cheersoundlist.cfg missing jeer sounds");
        delete kv;
        return;
    }

    for (int i = 0; i < MAXCHEERS; i++)
    {
        Format(buffer, sizeof(buffer), "%i", i + 1);
        kv.GetString(buffer, g_cCheerList[Jeer][i], PLATFORM_MAX_PATH);
        if (g_cCheerList[Jeer][i][0] != '\0')
        {
            Format(downloadFile, PLATFORM_MAX_PATH, "sound/%s", g_cCheerList[Jeer][i]);
            if (FileExists(downloadFile, true))
            {
                PrecacheSound(g_cCheerList[Jeer][i], true);
                AddFileToDownloadsTable(downloadFile);
                g_iCheerListNum[Jeer]++;
            }
        }
    }
    
    delete kv;
}

void PlaySound(int client, CheerType type)
{
    switch (view_as<int>(type))
    {
        case 0:
        {
            if (!IsValidClient(client))
                return;
            
            float vec[3];
            int rand = GetRandomInt(0, g_iCheerListNum[Cheer] - 1);
            GetClientEyePosition(client, vec);
            EmitAmbientSound(g_cCheerList[Cheer][rand], vec, SOUND_FROM_WORLD, SNDLEVEL_SCREAMING, _, g_cSoundVolume.FloatValue);
        }
        case 1:
        {
            int rand = GetRandomInt(0, g_iCheerListNum[Jeer] - 1);
            for (int i = 1; i <= MAXPLAYERS; i++)
                if (IsValidClient(i))
                    if(IsClientInGame(i))
                        EmitSoundToClient(i, g_cCheerList[Jeer][rand], _, _, _, _, g_cSoundVolume.FloatValue);
        }
    }
}

public Action CommandCheer(int client, int args)
{
    if(!client || !IsClientInGame(client))
        return Plugin_Handled;

    int cheerCount, jeerCount;
    SetRandomSeed(GetTime());

    char name[MAX_NAME_LENGTH_FIX];
    if (LibraryExists("NCS-Chat"))
        NCS_Chat_GetChatName(client, name, sizeof(name));
    else
        GetClientName(client, name, sizeof(name));

    cheerCount = g_cClientMaxCheers.IntValue;
    jeerCount = g_cClientMaxJeers.IntValue;

    if (LibraryExists("NCS-VIP"))
    {
        if (NCS_VIP_IsVIP(client))
        {
            cheerCount += g_cVIPCheersAdd.IntValue;
            jeerCount += g_cVIPJeersAdd.IntValue;
        }
    }

    if(IsPlayerAlive(client))
    {
        if (!g_iCheerListNum[Cheer]) return Plugin_Handled;

        if(g_iCheerCount[client][Cheer] >= cheerCount)
        {
            CPrintToChat(client, "\x04[系统提示]{blue} 每局只能欢呼 {red}%d{blue} 次", cheerCount);
            return Plugin_Handled;
        }

        char chat[MAX_CHAT_LENGTH];
        Format(chat, sizeof(chat), "%s   \x04%s", name, g_cCheerString[GetRandomInt(0, 4)]);
        ProcessColorString(chat, sizeof(chat), GetClientTeam(client));
        PrintToChatAll(chat);
        PlaySound(client, Cheer);
        
        g_iCheerCount[client][Cheer]++;
    }
    else
    {
        if (!g_iCheerListNum[Jeer]) return Plugin_Handled;

        if(g_iCheerCount[client][Jeer] >= jeerCount)
        {
            CPrintToChat(client, "\x04[系统提示]{blue} 每局死亡后只能嘲笑 {red}%d{blue} 次", jeerCount);
            return Plugin_Handled;
        }

        char chat[MAX_CHAT_LENGTH];
        Format(chat, sizeof(chat), "%s   \x04%s", name, g_cCheerString[GetRandomInt(0, 4)]);
        ProcessColorString(chat, sizeof(chat), GetClientTeam(client));
        PrintToChatAll(chat);
        PlaySound(client, Jeer);
        
        g_iCheerCount[client][Jeer]++;
    }
    
    return Plugin_Handled;
}

public Action CommandCheerTest(int client, int args)
{
    if(!client || !IsClientInGame(client))
        return Plugin_Handled;

    Menu menu = new Menu(MenuHandler_Cheer);
    menu.SetTitle("音效测试");

    for (int i = 0; i < g_iCheerListNum[Cheer]; i++)
        menu.AddItem(g_cCheerList[Cheer][i], g_cCheerList[Cheer][i]);

    for (int i = 0; i < g_iCheerListNum[Jeer]; i++)
        menu.AddItem(g_cCheerList[Jeer][i], g_cCheerList[Jeer][i]);
    
    menu.Display(client, MENU_TIME_FOREVER);
    
    return Plugin_Handled;
}

public int MenuHandler_Cheer(Menu menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Cancel)
    {
        if(param2 == MenuCancel_ExitBack)
            delete menu;
    }
    else if (action == MenuAction_Select)
    {
        char info[32];
        menu.GetItem(param2, info, sizeof(info));
        EmitSoundToClient(param1, info, _, _, _, _, g_cSoundVolume.FloatValue);
        menu.Display(param1, MENU_TIME_FOREVER);
    }
}
