void SetPlayerModelExt(int client, int skinID, bool immediately = false)
{
    if (skinID <= 0)
        return;

    Skin skin;
    if (!g_Skins.Get(skinID, skin))
        return;
    
    int body = GetUserSkinBody(client, skinID);
    SetPlayerModel(client, skin.skinPath, body, immediately);
    SkinSoundOnPlayerSpawn(client);
}

void ClearPlayerModel(int client)
{
    SetPlayerModel(client, "", 0);
}
