bool CheckUserInSkinGroup(int client, int skinID)
{
	static Skin skin;
	if (!g_Skins.Get(skinID, skin))
		return false;
	if (!skin.groupIDs.Length)
		return false;
	return CheckUserInSkinGroupEx(client, skin);
}

bool CheckUserInSkinGroupEx(int client, Skin skin)
{
	char gid[20];
	for (int i = 0; i < skin.groupIDs.Length; i++)
	{
		skin.groupIDs.GetString(i, gid, sizeof(gid));
		if (NCS_Account_InSteamGroup(client, gid))
			return true;
	}
	return false;
}

void AddUserGroupSkinMenu(Menu menu, int client, int &usedPosition, int type)
{
	bool admin = IsSkinAdmin(client);
	static Skin skin;
	StringMapSnapshot snapshot = g_Skins.Snapshot();
	int usedSkin;
	if (type == 0) usedSkin = g_UsersUsedSkin[client];
	if (type == 1) usedSkin = GetUserSkinArm(client);
	if (type == 2) usedSkin = GetUserSkinSound(client);
	int length = snapshot.Length;
	static char skinID[8]
	for (int i = 0; i < length; i++)
	{
		snapshot.GetKey(i, skinID, sizeof(skinID));
		g_Skins.GetArray(skinID, skin, sizeof(skin));
		if (!skin.groupIDs.Length)
			continue;
		if (type == 1 && !skin.armPath[0]) continue;
		if (type == 2 && !skin.sounds.Size) continue;
		if (!CheckUserInSkinGroupEx(client, skin) && !admin)
			continue;
		char desc[64];
		Format(desc, sizeof(desc), "%s", skin.name);
		bool used = usedSkin == skin.id;
		if (used)
			Format(desc, sizeof(desc), "(使用中)%s", desc);
		menu.AddItem(skinID, desc);
		if (used) usedPosition = menu.ItemCount - 1;
	}
	delete snapshot;
}