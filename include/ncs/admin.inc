#if defined _NCS_Admin_
	#endinput
#endif
#define _NCS_Admin_

stock bool IsAdmin(int client)
{
    return (GetUserFlagBits(client) & ADMFLAG_GENERIC) == 1;
}

stock bool IsOwner(int client)
{
    return (GetUserFlagBits(client) & ADMFLAG_ROOT) == 1;
}