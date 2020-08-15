#define SPRAY_COOLDOWN 30

static int playerLimiter[MAXPLAYERS+1];

void CreateSprays(int client, int id = 0, bool preview = false)
{
    if (!IsClientInGame(client) || !IsPlayerAlive(client) || IsFakeClient(client))
        return;

    if (!preview)
    {
        id = g_UsersUsedSpray[client];
        if (id == 0)
            return;
        if (playerLimiter[client] > GetTime())
            return;
    }

    float m_flEye[3];
    GetClientEyePosition(client, m_flEye);

    float m_flView[3];
    GetPlayerEyeViewPoint(client, m_flView);
    
    float distance = GetVectorDistance(m_flEye, m_flView);
    if(distance > 115.0)
        return;

    Spray s;
    if (!sprays.Get(id, s))
        return;

    TE_Start("World Decal");
    TE_WriteVector("m_vecOrigin",m_flView);
    TE_WriteNum("m_nIndex", s.decal);

    if (preview)
    {
        TE_SendToClient(client);
        EmitSoundToClient(client, SPRAY_SOUND, _, _, _, _, 1.0);
    }
    else
    { 
        TE_SendToAll();
        EmitSoundToAll(SPRAY_SOUND, client, _, _, _, 1.0);
        playerLimiter[client] = GetTime() + SPRAY_COOLDOWN;
    }
}

void GetPlayerEyeViewPoint(int client, float m_fPosition[3])
{
    float m_flRotation[3];
    float m_flPosition[3];

    GetClientEyeAngles(client, m_flRotation);
    GetClientEyePosition(client, m_flPosition);

    TR_TraceRayFilter(m_flPosition, m_flRotation, MASK_ALL, RayType_Infinite, TraceRayDontHitSelf, client);
    TR_GetEndPosition(m_fPosition);
}

public bool TraceRayDontHitSelf(int entity, int mask, any data)
{
    if(entity == data)
        return false;
    return true;
}

void PlayerOnRunCmd(int client, int impulse)
{
    if (impulse == 201)
        CreateSprays(client);
}