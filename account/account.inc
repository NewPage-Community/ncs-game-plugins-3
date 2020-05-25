
enum struct account
{
	int index;
	int uid;
	char steamid[32];

	bool Valid() {
		char steamid[32];
		GetClientAuthId(this.index, AuthId_SteamID64, steamid, sizeof(steamid), true);
		return IsValidClient(this.index) && this.uid > 0 && StrEqual(steamid, this.steamid)
	}

	void SetSteamID(const char[] s) {
		strcopy(this.steamid, 32, s);
	}

	void GetSteamID(char[] steamid, int size) {
		strcopy(steamid, size, this.steamid);
	}

	bool Clean() {
		this.index = 0;
		this.uid = 0;
		this.steamid = "";
	}
}

account g_Accounts[MAXPLAYERS + 1];

#include "account/api"
#include "account/check"
#include "account/cmd"
#include "account/native"