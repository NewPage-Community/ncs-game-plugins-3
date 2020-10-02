#include <clients>

// Check client validation
stock bool IsValidClient(int index)
{
    return (index > 0 && index <= MaxClients && IsClientConnected(index) && !IsFakeClient(index) && !IsClientSourceTV(index));
}

stock int FindClientBySteamId(AuthIdType authType, const char[] authId, bool validate = true)
{
    char steamId[32];
    for(int client = 1; client <= MaxClients; client++)
        if(IsValidClient(client))
            if(GetClientAuthId(client, authType, steamId, 32, validate))
                if(strcmp(steamId, authId) == 0)
                    return client;

    return 0;
}

stock int GetHumanCount(bool inGame = false)
{
    int count = 0;
    for (int client = 1; client <= MaxClients; client++)
    {
        if (IsClientConnected(client) && !IsFakeClient(client))
        {
            if (inGame)
            {
                if (IsClientInGame(client))
                    count++;
            }
            else
                count++;
        }
    }
    return count;
}