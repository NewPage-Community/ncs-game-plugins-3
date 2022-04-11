#if defined _InsExt_included
 #endinput
#endif
#define _InsExt_included


public Extension __ext_InsExt =
{
	name = "InsExt",
	file = "InsExt.ext",
	autoload = 1,
	required = 1,
};

methodmap CBaseAnimating {
	public CBaseAnimating(int entity) {
		return view_as<CBaseAnimating>(entity);
	}
	public native int GetBodyGroup(int body, int iGroup);
	public native int GetBodyGroupCount(int iGroup);
	public native bool SetBodyGroup(int body, int iGroup);
}

native bool SetPlayerModel(int client, const char[] name, int body, int skin, bool immediately = false);
native void SetPlayerHands(int client, const char[] name);
native int GetHandsGearID(char[] name);
