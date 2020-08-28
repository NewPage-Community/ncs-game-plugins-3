#include <ncs/chat>
#include <ncs/client>

#pragma semicolon 1
#pragma newdecls required

#define MinKickClients 0
#define CheckPlayerTimer 600.0
#define CheckCountToKick 3

public Plugin myinfo = 
{
	name        = "AntiAFK",
	author      = "Takatoshi, Kyle",
	description = "Judging the status of users and kick the clients maliciously hanging up.",
	version     = "1.0",
	url         = ""
};

enum struct client_status_info
{
    bool  m_IsAFK;
    int   m_Count;
    float m_Angle;
    float m_Vel;
}

static client_status_info g_Client[MAXPLAYERS+1];

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    RegPluginLibrary("AntiAFK");
    return APLRes_Success;
}

public void OnMapStart()
{
    CreateTimer(CheckPlayerTimer, Timer_CheckPlayers, _, TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
}

public void OnClientConnected(int client)
{
    resetPlayer(client);
}

public Action Timer_CheckPlayers(Handle timer, any unused)
{
    if (GetClientCount(false) <= MinKickClients)        // There's no need to kick a player when the server haven't filled yet.
        return Plugin_Continue;

    for(int client = 1; client <= MaxClients; ++client)        //To search every client in the server
    {
        if (!IsValidClient(client))     // The Client is valid then go on 
            continue;
        
        if (g_Client[client].m_IsAFK)   //Default m_IsAFK is 'false'
        {
            if (++g_Client[client].m_Count >= CheckCountToKick)    //more than 5 times then kick out
            {
                KickClient(client, "[AFK] 挂机时间过长被踢出");
                continue;
            }
            NCS_Chat(client, "[AFK]", "{red}警告{default}:  {blue}长时间无操作将会被踢出游戏");
        }
        g_Client[client].m_IsAFK = true;
    }

    return Plugin_Continue;
}

public void OnPlayerRunCmdPost(int client, int buttons, int impulse, const float vel[3], const float angles[3], int weapon, int subtype, int cmdnum, int tickcount, int seed, const int mouse[2])
{
    if (IsFakeClient(client))
        return;

    if (mouse[0] || mouse[1] || g_Client[client].m_Angle != angles[0] || g_Client[client].m_Vel != vel[1])
    {
        // reset
        resetPlayer(client);
    }

    g_Client[client].m_Angle = angles[0];
    g_Client[client].m_Vel = vel[1];
}

static void resetPlayer(int client)
{
    g_Client[client].m_IsAFK = false;
    g_Client[client].m_Count = 0;
}