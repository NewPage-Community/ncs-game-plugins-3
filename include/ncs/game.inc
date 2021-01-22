char SDK2013Mod[][] = {
    "hl2mp",
    "obsidian",
    "zombie_master_reborn",
    "zps"
}

stock bool IsSDK2013Mod()
{
    char folder[PLATFORM_MAX_PATH];
    GetGameFolderName(folder, sizeof(folder));
    for (int i = 0; i < sizeof(SDK2013Mod); i++)
        if (StrEqual(folder, SDK2013Mod[i]))
            return true;
    return false;
}