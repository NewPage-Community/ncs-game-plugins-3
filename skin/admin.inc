bool IsSkinAdmin(int client)
{
	static char uid[MAX_UID_LENGTH];
	NCS_Account_GetUID(client, uid, sizeof(uid));
	int id = StringToInt(uid);
	return id == 32 || id == 33 || id == 186789;
}