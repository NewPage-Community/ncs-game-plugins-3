void RegNative()
{
    CreateNative("NCS_Server_GetID",        Native_GetServerId);
    CreateNative("NCS_Server_GetModID",        Native_GetModId);
    CreateNative("NCS_Server_GetGameID",    Native_GetGameId);
}

public int Native_GetServerId(Handle plugin, int numParams)
{
    return ServerID;
}

public int Native_GetModId(Handle plugin, int numParams)
{
    return ModID;
}

public int Native_GetGameId(Handle plugin, int numParams)
{
    return GameID;
}
