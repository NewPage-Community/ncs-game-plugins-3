#if defined _NCS_Stats_
	#endinput
#endif
#define _NCS_Stats_

native void NCS_Stats_Add(int client, const char[] range, const char[] stats_name, const char[] version, int increment);

public SharedPlugin __pl_ncs_stats = 
{
    name = "NCS-Stats",
    file = "np-stats.smx",
    required = 1,
};