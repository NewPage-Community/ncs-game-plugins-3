void SetPlayerModelExt(int client, int skinID, bool immediately = false, bool sound = false)
{
    if (skinID <= 0)
        return;

    Skin skin;
    if (!g_Skins.Get(skinID, skin))
        return;
    
    int body = GetUserSkinBody(client, skinID);
    if (body == -1) body = skin.initBody;
    SetPlayerModel(client, skin.skinPath, body, immediately);
    if (skin.armPath[0]) SetPlayerHands(client, skin.armPath);
    if (IsClientInGame(client) && sound)
    {
        SkinSoundOnPlayerSpawn(client);
    }
}

void ClearPlayerModel(int client)
{
    SetPlayerModel(client, "", 0);
    SetPlayerHands(client, "");
}