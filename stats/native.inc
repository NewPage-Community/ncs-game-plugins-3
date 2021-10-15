void InitNative()
{
    CreateNative("NCS_Stats_Add", Native_Add);
    CreateNative("NCS_Stats_Get", Native_Get);
}

public int Native_Add(Handle plugin, int numParams)
{
    StatsInfo stats;
    int client = GetNativeCell(1);

    if (!NCS_Account_GetUID(client, stats.uid, MAX_UID_LENGTH))
        return 0;
    stats.increment = GetNativeCell(5);
    GetNativeString(2, stats.range, 8);
    GetNativeString(3, stats.stats_name, 8);
    GetNativeString(4, stats.version, 8);

    StatsList.Add(stats);
    return 1;
}

public int Native_Get(Handle plugin, int numParams)
{
    char range[NCS_STATS_KEY_LENGTH], statsName[NCS_STATS_KEY_LENGTH], version[NCS_STATS_KEY_LENGTH];
    int client = GetNativeCell(1);
    Function callback = GetNativeFunction(2);
    GetNativeString(3, range, sizeof(range));
    GetNativeString(4, statsName, sizeof(statsName));
    GetNativeString(5, version, sizeof(version));
    GetStatsData(client, plugin, callback, range, statsName, version);
    return 1;
}