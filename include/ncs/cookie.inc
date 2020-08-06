#if defined _NCS_Cookie_
	#endinput
#endif
#define _NCS_Cookie_

#define MAX_COOKIE_KEY_LENGTH 64
#define MAX_COOKIE_VALUE_LENGTH 256

forward void NCS_Cookie_OnUserCached(int client);

native bool NCS_Cookie_Get(int client, const char[] key, char[] value, int maxlen);
native void NCS_Cookie_Set(int client, const char[] key, const char[] value);

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