void LoadAdmin(int client, int flagbits, int immunity)
{
	char auth[32];
	if (!GetClientAuthId(client, AuthId_Steam2, auth, sizeof(auth), false))
		return;

	AdminId admin;

	if((admin = FindAdminByIdentity(AUTHMETHOD_STEAM, auth)) != INVALID_ADMIN_ID)
	{
		RemoveAdmin(admin);
		admin = INVALID_ADMIN_ID;
	}
		
	if((admin = CreateAdmin(auth)) != INVALID_ADMIN_ID)
	{
		if(!admin.BindIdentity(AUTHMETHOD_STEAM, auth))
		{
			NCS_LogError("Admin", "InitAdmin", "Unable to bind admin %s to identity", auth);
			RemoveAdmin(admin);
			return;
		}

		admin.ImmunityLevel = immunity;

		AdminFlag flags[AdminFlags_TOTAL];
		int flags_num = FlagBitsToArray(flagbits, flags, AdminFlags_TOTAL);

		for (int i = 0; i < flags_num; i++)
			SetAdminFlag(admin, flags[i], true);

		if(IsClientInGame(client))
			RunAdminCacheChecks(client);
	}
}

#include "admin/api"