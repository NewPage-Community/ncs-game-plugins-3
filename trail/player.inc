static playerTrail[MAXPLAYERS+1];
static int g_iPreviewEnt[2048];
static Handle g_tPreview[MAXPLAYERS+1];

void SetClientTrail(int client, int id = 0, bool preview = false)
{
    if (!IsClientInGame(client) || !IsPlayerAlive(client) || IsFakeClient(client))
        return;
    
    // For Insurgency PVE
    if (GetClientTeam(client) != 2)
        return;

    Handle timer = g_tPreview[client];
    if(timer != INVALID_HANDLE && IsValidHandle(timer))
        KillTimer(timer);

    if (playerTrail[client] != INVALID_ENT_REFERENCE)
        RemoveClientTrail(client);

    if (!preview)
    {
        id = g_UsersUsedTrail[client];
        if (id == 0)
            return;
    }

    Trail s;
    if (!trails.Get(id, s))
        return;

    int entity;
    switch (s.type)
    {
        case TrailType_Material: entity = CreateSpriteTrail(client, s.trailPath);
        case TrailType_Particle: entity = CreateParticleTrail(client, s.particleName);
    }

    playerTrail[client] = EntIndexToEntRef(entity);

    if (preview)
    {
        g_iPreviewEnt[entity] = client;
        SDKHook(entity, SDKHook_SetTransmit, Hook_SetTransmit_Preview);
        g_tPreview[client] = CreateTimer(30.0, Timer_ClearPreTrail, client);
        NCS_Chat(client, _, "{blue}预览足迹 {lime}%s{blue} 30秒，看着脚下走两步查看效果", s.name);
    }
}

int CreateSpriteTrail(int client, const char[] path)
{
    int entity = CreateEntityByName("env_spritetrail");
    DispatchKeyValue(entity, "classname", "env_spritetrail");
    DispatchKeyValue(entity, "renderamt", "255");
    DispatchKeyValue(entity, "rendercolor", "255 255 255");
    DispatchKeyValue(entity, "lifetime", "1.0");
    DispatchKeyValue(entity, "rendermode", "5");
    DispatchKeyValue(entity, "spritename", path);
    DispatchKeyValue(entity, "startwidth", "10.0");
    DispatchKeyValue(entity, "endwidth", "10.0");
    SetEntPropFloat(entity, Prop_Send, "m_flTextureRes", 0.05);
    DispatchSpawn(entity);

    float m_fOrigin[3];
    float m_fAngle[3];
    float m_fTemp[3] = {0.0, 90.0, 0.0};
    GetEntPropVector(client, Prop_Data, "m_angAbsRotation", m_fAngle);
    SetEntPropVector(client, Prop_Data, "m_angAbsRotation", m_fTemp);
    float m_fPosition[3];
    m_fPosition[0] = 0.0;
    m_fPosition[1] = 0.0;
    m_fPosition[2]= 5.0;
    GetClientAbsOrigin(client, m_fOrigin);
    AddVectors(m_fOrigin, m_fPosition, m_fOrigin);
    TeleportEntity(entity, m_fOrigin, m_fTemp, NULL_VECTOR);
    SetVariantString("!activator");
    AcceptEntityInput(entity, "SetParent", client, entity);
    SetEntPropVector(client, Prop_Data, "m_angAbsRotation", m_fAngle);
    
    SetVariantString("OnUser1 !self:SetScale:1:0.5:-1");
    AcceptEntityInput(entity, "AddOutput");
    AcceptEntityInput(entity, "FireUser1");
    return entity;
}

int CreateParticleTrail(int client, const char[] name)
{
    float clientOrigin[3];
    GetClientAbsOrigin(client, clientOrigin);

    int entity = CreateEntityByName("info_particle_system");
    DispatchKeyValue(entity, "start_active", "1");
    DispatchKeyValue(entity, "effect_name", name);
    DispatchSpawn(entity);
    TeleportEntity(entity, clientOrigin, NULL_VECTOR, NULL_VECTOR);
    SetVariantString("!activator");
    AcceptEntityInput(entity, "SetParent", client, entity, 0);
    ActivateEntity(entity);
    return entity;
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
    SetClientTrail(client);
    return Plugin_Stop;
}

public Action Hook_SetTransmit_Preview(int ent, int client)
{
    return (g_iPreviewEnt[ent] == client) ? Plugin_Continue : Plugin_Handled;
}