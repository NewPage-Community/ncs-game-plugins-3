static playerTrail[MAXPLAYERS+1];
static int g_iPreviewEnt[2048];
static Handle g_tPreview[MAXPLAYERS+1];

void CreateTrail(int client, int id = 0, bool preview = false)
{
    if (!IsClientInGame(client) || !IsPlayerAlive(client) || IsFakeClient(client))
        return;
    
    // For Insurgency PVE
    if (GetClientTeam(client) != 2)
        return;

    if (!preview)
    {
        id = g_UsersUsedTrail[client];
        if (id == 0)
            return;
    }

    Trail s;
    if (!trails.Get(id, s))
        return;

    if(g_tPreview[client] != INVALID_HANDLE)
        KillTimer(g_tPreview[client]);

    if (playerTrail[client] == INVALID_ENT_REFERENCE)
        RemoveClientTrail(client);

    int entity = CreateEntityByName("env_spritetrail");
    DispatchKeyValue(entity, "classname", "env_spritetrail");
    DispatchKeyValue(entity, "renderamt", "255");
    DispatchKeyValue(entity, "rendercolor", "255 255 255");
    DispatchKeyValue(entity, "lifetime", "1.0");
    DispatchKeyValue(entity, "rendermode", "5");
    DispatchKeyValue(entity, "spritename", s.trailPath);
    DispatchKeyValue(entity, "startwidth", "10.0");
    DispatchKeyValue(entity, "endwidth", "10.0");
    SetEntPropFloat(entity, Prop_Send, "m_flTextureRes", 0.05);
    DispatchSpawn(entity);
    AttachTrail(entity, client, 0, 0);    

    playerTrail[client] = EntIndexToEntRef(entity);

    if (preview)
    {
        g_iPreviewEnt[entity] = client;
        SDKHook(entity, SDKHook_SetTransmit, Hook_SetTransmit_Preview);
        g_tPreview[client] = CreateTimer(30.0, Timer_ClearPreTrail, client);
    }
}

void AttachTrail(int ent, int client, int current, int num)
{
    float m_fOrigin[3];
    float m_fAngle[3];
    float m_fTemp[3] = {0.0, 90.0, 0.0};
    GetEntPropVector(client, Prop_Data, "m_angAbsRotation", m_fAngle);
    SetEntPropVector(client, Prop_Data, "m_angAbsRotation", m_fTemp);
    float m_fX = (30.0*((num-1)%3))/2-(30.0*(current%3));
    float m_fPosition[3];
    m_fPosition[0] = m_fX;
    m_fPosition[1] = 0.0;
    m_fPosition[2]= 5.0+(current/3)*30.0;
    GetClientAbsOrigin(client, m_fOrigin);
    AddVectors(m_fOrigin, m_fPosition, m_fOrigin);
    TeleportEntity(ent, m_fOrigin, m_fTemp, NULL_VECTOR);
    SetVariantString("!activator");
    AcceptEntityInput(ent, "SetParent", client, ent);
    SetEntPropVector(client, Prop_Data, "m_angAbsRotation", m_fAngle);
    
    SetVariantString("OnUser1 !self:SetScale:1:0.5:-1");
    AcceptEntityInput(ent, "AddOutput");
    AcceptEntityInput(ent, "FireUser1");
}

void RemoveClientTrail(int client)
{
    if(playerTrail[client] != INVALID_ENT_REFERENCE)
    {
        int entity = EntRefToEntIndex(playerTrail[client]);
        if(entity > 0 && IsValidEdict(entity))
        {
            AcceptEntityInput(entity, "Kill");
        }
    }

    playerTrail[client] = INVALID_ENT_REFERENCE;
}

public Action Timer_ClearPreTrail(Handle timer, int client)
{
    g_tPreview[client] = INVALID_HANDLE;
    RemoveClientTrail(client);
    return Plugin_Stop;
}

public Action Hook_SetTransmit_Preview(int ent, int client)
{
    return (g_iPreviewEnt[ent] == client) ? Plugin_Continue : Plugin_Handled;
}