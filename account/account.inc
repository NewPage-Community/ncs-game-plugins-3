#define MAX_STEAMID_LENGTH 32

enum struct account
{
	int index;
	int uid;
	char steamid[MAX_STEAMID_LENGTH];
	char name[MAX_NAME_LENGTH];

	bool Valid() {
		if (this.index == 0 || this.uid <= 0)
			return false;
		char steamid[MAX_STEAMID_LENGTH];
		GetClientAuthId(this.index, AuthId_SteamID64, steamid, sizeof(steamid), true);
		return IsValidClient(this.index) && StrEqual(steamid, this.steamid)
	}

	void Init(int index, int uid, const char[] steamid) {
		this.index = index;
		this.uid = uid;
		strcopy(this.steamid, MAX_STEAMID_LENGTH, steamid);
		GetClientName(index, this.name, MAX_NAME_LENGTH);
	}

	void UpdateName(const char[] username) {
		if (!this.Valid())
			return;
		strcopy(this.name, MAX_STEAMID_LENGTH, username);
	}

	bool Clean() {
		this.index = 0;
		this.uid = 0;
		this.steamid = "";
		this.name = "";
	}
}

account g_Accounts[MAXPLAYERS + 1];

#include "account/name"
#include "account/api"
#include "account/check"
#include "account/cmd"
#include "account/native"