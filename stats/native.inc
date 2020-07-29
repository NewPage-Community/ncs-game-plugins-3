void InitNative()
{
    CreateNative("NCS_Stats_Add", Native_Add);
}

public int Native_Add(Handle plugin, int numParams)
{
    StatsInfo stats;
    int client = GetNativeCell(1);

    if (!NCS_Account_GetUID(client, stats.uid, MAX_UID_LENGTH))
        return;
    stats.increment = GetNativeCell(5);
    GetNativeString(2, stats.range, 8);
    GetNativeString(3, stats.stats_name, 8);
    GetNativeString(4, stats.version, 8);

    StatsList.Add(stats);
}