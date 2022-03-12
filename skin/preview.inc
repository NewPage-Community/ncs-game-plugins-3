static int g_iPreviewEnt[2048];
static int g_iPreMdlRef[MAXPLAYERS+1] = {INVALID_ENT_REFERENCE, ...};
static Handle g_tPreMdl[MAXPLAYERS+1];

void PreviewSkin(int client, int skinID)
{
    if(skinID <= 0)
        return;

    Skin s;
    if (!g_Skins.Get(skinID, s))
        return;
    
    if(g_tPreMdl[client] != INVALID_HANDLE)
        KillTimer(g_tPreMdl[client]);

    Timer_ClearPreMdl(INVALID_HANDLE, client);
    
    int m_iViewModel = CreateEntityByName("prop_dynamic_override");
    
    char m_szTargetName[32];
    FormatEx(m_szTargetName, 32, "Skin_Preview_%d", m_iViewModel);
    DispatchKeyValue(m_iViewModel, "targetname", m_szTargetName);

    DispatchKeyValue(m_iViewModel, "spawnflags", "64");
    DispatchKeyValue(m_iViewModel, "model", s.skinPath);
    DispatchKeyValue(m_iViewModel, "rendermode", "0");
    DispatchKeyValue(m_iViewModel, "renderfx", "0");
    DispatchKeyValue(m_iViewModel, "rendercolor", "255 255 255");
    DispatchKeyValue(m_iViewModel, "renderamt", "255");
    DispatchKeyValue(m_iViewModel, "solid", "0");
    
    DispatchSpawn(m_iViewModel);
    
    SetEntProp(m_iViewModel, Prop_Send, "m_CollisionGroup", 11);
    SetEntProp(m_iViewModel, Prop_Send, "m_nBody", s.initBody);
    
    AcceptEntityInput(m_iViewModel, "Enable");
    
    if (HasEntProp(m_iViewModel, Prop_Send, "m_bShouldGlow"))
    {
        int offset = GetEntSendPropOffs(m_iViewModel, "m_clrGlow");
        SetEntProp(m_iViewModel, Prop_Send, "m_bShouldGlow", true, true);
        SetEntProp(m_iViewModel, Prop_Send, "m_nGlowStyle", 0);
        SetEntPropFloat(m_iViewModel, Prop_Send, "m_flGlowMaxDist", 2000.0);

        //Miku Green
        SetEntData(m_iViewModel, offset    ,  57, _, true);
        SetEntData(m_iViewModel, offset + 1, 197, _, true);
        SetEntData(m_iViewModel, offset + 2, 187, _, true);
        SetEntData(m_iViewModel, offset + 3, 255, _, true);
    }

    float m_fOrigin[3], m_fAngles[3], m_fRadians[2], m_fPosition[3];

    GetClientAbsOrigin(client, m_fOrigin);
    GetClientAbsAngles(client, m_fAngles);

    m_fRadians[0] = DegToRad(m_fAngles[0]);
    m_fRadians[1] = DegToRad(m_fAngles[1]);

    m_fPosition[0] = m_fOrigin[0] + 64 * Cosine(m_fRadians[0]) * Cosine(m_fRadians[1]);
    m_fPosition[1] = m_fOrigin[1] + 64 * Cosine(m_fRadians[0]) * Sine(m_fRadians[1]);
    m_fPosition[2] = m_fOrigin[2] + 4 * Sine(m_fRadians[0]);
    
    m_fAngles[0] *= -1.0;
    m_fAngles[1] *= -1.0;

    TeleportEntity(m_iViewModel, m_fPosition, m_fAngles, NULL_VECTOR);

    g_iPreMdlRef[client] = EntIndexToEntRef(m_iViewModel);

    SDKHookEx(m_iViewModel, SDKHook_SetTransmit, Hook_SetTransmit_Preview);
    g_iPreviewEnt[m_iViewModel] = client;
    
    g_tPreMdl[client] = CreateTimer(30.0, Timer_ClearPreMdl, client);

    NCS_Chat(client, _, "预览皮肤 {lime}%s{default} 30秒", s.name);
}

public Action Timer_ClearPreMdl(Handle timer, int client)
{
    g_tPreMdl[client] = INVALID_HANDLE;
    
    if(g_iPreMdlRef[client] != INVALID_ENT_REFERENCE)
    {
        int entity = EntRefToEntIndex(g_iPreMdlRef[client]);

        if(IsValidEdict(entity))
        {
            g_iPreviewEnt[entity] = 0;
            SDKUnhook(entity, SDKHook_SetTransmit, Hook_SetTransmit_Preview);
            AcceptEntityInput(entity, "Kill");
        }
        
        g_iPreMdlRef[client] = INVALID_ENT_REFERENCE;
    }

    return Plugin_Stop;
}

public Action Hook_SetTransmit_Preview(int ent, int client)
{
    return (g_iPreviewEnt[ent] == client) ? Plugin_Continue : Plugin_Handled;
}

void Preview_PlayerSpawn(int client)
{
    if(g_tPreMdl[client] != INVALID_HANDLE)
        KillTimer(g_tPreMdl[client]);
    g_tPreMdl[client] = INVALID_HANDLE;
}