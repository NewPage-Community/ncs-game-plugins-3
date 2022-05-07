void SetPlayerModelExt(int client, int skinID, bool immediately = false, bool sound = false)
{
	if (skinID <= 0)
		return;

	Skin skin;
	if (!g_Skins.Get(skinID, skin))
		return;
	
	int body = GetUserSkinBody(client, skinID);
	if (body == -1) body = skin.initBody;
	int skinSkin = GetUserSkinSkin(client, skinID);
	if (skinSkin == -1) skinSkin = 0;
	SetPlayerModel(client, skin.skinPath, body, skinSkin, immediately);
	if (skin.armPath[0] && !SkinArmsOff(client)) SetPlayerHands(client, skin.armPath);
	if (IsClientInGame(client) && sound)
	{
		SkinSoundOnPlayerSpawn(client);
	}
}

void ClearPlayerModel(int client)
{
	SetPlayerModel(client, "", 0, 0);
	SetPlayerHands(client, "");
}

void Model_PlayerSpawn(int client)
{
	int skinID = g_UsersUsedSkin[client];
	if (!skinID) return;

	Skin skin;
	if (!g_Skins.Get(skinID, skin)) return;

	SetEntProp(client, Prop_Send, "m_nSkin", GetUserSkinBody(client, skinID));
	SetEntProp(client, Prop_Send, "m_nSkin", GetUserSkinSkin(client, skinID));
}
