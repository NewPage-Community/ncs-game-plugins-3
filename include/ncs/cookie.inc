#if defined _NCS_Cookie_
	#endinput
#endif
#define _NCS_Cookie_

#define MAX_COOKIE_KEY_LENGTH 64
#define MAX_COOKIE_VALUE_LENGTH 256

forward void NCS_Cookie_OnUserCached(int client);

native bool NCS_Cookie_Get(int client, const char[] key, char[] value, int maxlen);
native void NCS_Cookie_Set(int client, const char[] key, const char[] value);

native void NCS_Cookie_IsCached(int client);

stock bool NCS_Cookie_GetBool(int client, const char[] key, bool def)
{
    char v[2];
    if (NCS_Cookie_Get(client, key, v, sizeof(v)))
        return StringToInt(v) == 1;
    return def;
}

stock void NCS_Cookie_SetBool(int client, const char[] key, bool value)
{
    char v[2];
    IntToString(value, v, sizeof(v));
    NCS_Cookie_Set(client, key, v);
}

stock int NCS_Cookie_GetInteger(int client, const char[] key, int def)
{
    char v[20];
    if (NCS_Cookie_Get(client, key, v, sizeof(v)))
        return StringToInt(v);
    return def;
}

stock void NCS_Cookie_SetInteger(int client, const char[] key, int value)
{
    char v[20];
    IntToString(value, v, sizeof(v));
    NCS_Cookie_Set(client, key, v);
}

public SharedPlugin __pl_ncs_cookie = 
{
    name = "NCS-Cookie",
    file = "np-cookie.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_cookie_SetNTVOptional()
{
    MarkNativeAsOptional("NCS_Cookie_Get");
	MarkNativeAsOptional("NCS_Cookie_Set");
}
#endif