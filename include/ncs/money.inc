#if defined _NCS_Money_
	#endinput
#endif
#define _NCS_Money_

native void NCS_Money_Give(int client, int money, const char[] reason);

public SharedPlugin __pl_ncs_money = 
{
    name = "NCS-Money",
    file = "np-money.smx",
    required = 1,
};