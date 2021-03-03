void InitNative()
{
    CreateNative("NCS_Cookie_Get", Native_GetClient);
    CreateNative("NCS_Cookie_Set", Native_SetClient);
    CreateNative("NCS_Cookie_IsCached", Native_IsCached);
}

public int Native_GetClient(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    int length = GetNativeCell(4);

    char key[MAX_COOKIE_KEY_LENGTH], value[MAX_COOKIE_VALUE_LENGTH];
    GetNativeString(2, key, sizeof(key));

    bool ok = player[client].Get(key, value, sizeof(value));
    SetNativeString(3, value, length);
    return ok;
}

public int Native_SetClient(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);

    char key[MAX_COOKIE_KEY_LENGTH], value[MAX_COOKIE_VALUE_LENGTH];
    GetNativeString(2, key, sizeof(key));
    GetNativeString(3, value, sizeof(value));
    
    return SetUserCookie(client, key, value);
}

public int Native_IsCached(Handle plugin, int numParams)
{
    return player[GetNativeCell(1)].cached;
}