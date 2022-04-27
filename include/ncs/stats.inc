#if defined _NCS_Stats_
    #endinput
#endif
#define _NCS_Stats_

#define NCS_STATS_KEY_LENGTH 16

enum struct StatsData
{
    char range[NCS_STATS_KEY_LENGTH];
    char statsName[NCS_STATS_KEY_LENGTH];
    char version[NCS_STATS_KEY_LENGTH];
    float score;
    int rank;
    int total;
}

typeset StatsDataCallback
{
    function void (int client, float score, int rank, int total);
    function void (int client, float score, int rank, int total, any data);
};

typeset GlobalStatsDataCallback
{
    function void (float score);
    function void (float score, any data);
};

native void NCS_Stats_Add(int client, const char[] range, const char[] statsName, const char[] version, float increment);
native void NCS_Stats_Get(int client, StatsDataCallback callback, const char[] range, const char[] statsName, const char[] version, any data = 0);

native void NCS_Stats_Global_Add(const char[] range, const char[] statsName, const char[] version, float increment);
native void NCS_Stats_Global_Get(GlobalStatsDataCallback callback, const char[] range, const char[] statsName, const char[] version, any data = 0);

public SharedPlugin __pl_ncs_stats = 
{
    name = "NCS-Stats",
    file = "np-stats.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_stats_SetNTVOptional()
{
    MarkNativeAsOptional("NCS_Stats_Add");
    MarkNativeAsOptional("NCS_Stats_Get");
}
#endif