#if defined _NCS_Money_
	#endinput
#endif
#define _NCS_Money_

native void NCS_Money_Give(int client, int money, const char[] reason);

public SharedPlugin __pl_ncs_money = 
{
    name = "NCS-Money",
    file = "np-money.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_ncs_money_SetNTVOptional()
{
    NCS_Money_Give("NCS_Money_Give");
}
#endif