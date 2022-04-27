void InitNative()
{
    CreateNative("NCS_Stats_Add", Native_Add);
    CreateNative("NCS_Stats_Get", Native_Get);
    CreateNative("NCS_Stats_Global_Add", Native_Global_Add);
    CreateNative("NCS_Stats_Global_Get", Native_Global_Get);
}

public int Native_Add(Handle plugin, int numParams)
{
    static StatsInfo stats;
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
    static char range[NCS_STATS_KEY_LENGTH], statsName[NCS_STATS_KEY_LENGTH], version[NCS_STATS_KEY_LENGTH];
    int client = GetNativeCell(1);
    Function callback = GetNativeFunction(2);
    GetNativeString(3, range, sizeof(range));
    GetNativeString(4, statsName, sizeof(statsName));
    GetNativeString(5, version, sizeof(version));
    any data = GetNativeCell(6);
    GetStatsData(client, plugin, callback, range, statsName, version, data);
    return 1;
}

public int Native_Global_Add(Handle plugin, int numParams)
{
    static char range[NCS_STATS_KEY_LENGTH], statsName[NCS_STATS_KEY_LENGTH], version[NCS_STATS_KEY_LENGTH];
    GetNativeString(1, range, sizeof(range));
    GetNativeString(2, statsName, sizeof(statsName));
    GetNativeString(3, version, sizeof(version));
    AddGlobalStatsData(range, statsName, version, GetNativeCell(4));
    return 1;
}

public int Native_Global_Get(Handle plugin, int numParams)
{
    static char range[NCS_STATS_KEY_LENGTH], statsName[NCS_STATS_KEY_LENGTH], version[NCS_STATS_KEY_LENGTH];
    Function callback = GetNativeFunction(1);
    GetNativeString(2, range, sizeof(range));
    GetNativeString(3, statsName, sizeof(statsName));
    GetNativeString(4, version, sizeof(version));
    any data = GetNativeCell(5);
    GetGlobalStatsData(plugin, callback, range, statsName, version, data);
    return 1;
}