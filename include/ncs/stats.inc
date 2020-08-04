#if defined _NCS_Stats_
	#endinput
#endif
#define _NCS_Stats_

native void NCS_Stats_Add(int client, const char[] range, const char[] stats_name, const char[] version, float increment);

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
}
#endif