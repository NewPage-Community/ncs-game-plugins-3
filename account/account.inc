#define MAX_STEAMID_LENGTH INT64_LENGTH
#define MAX_UID_LENGTH INT64_LENGTH

enum struct account
{
	int index;
	char uid[MAX_UID_LENGTH];
	char steamid[MAX_STEAMID_LENGTH];
	char name[MAX_NAME_LENGTH];

	bool Valid() {
		if (this.index == 0 || this.uid[0] == '\0')
			return false;
		char steamid[MAX_STEAMID_LENGTH];
		GetClientAuthId(this.index, AuthId_SteamID64, steamid, sizeof(steamid), true);
		return IsValidClient(this.index) && StrEqual(steamid, this.steamid)
	}

	void Init(int index, const char[] uid, const char[] steamid) {
		this.index = index;
		strcopy(this.uid, MAX_UID_LENGTH, uid);
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
		this.uid[0] = '\0';
		this.steamid[0] = '\0';
		this.name[0] = '\0';
	}
}

account g_Accounts[MAXPLAYERS + 1];

#include "account/name"
#include "account/api"
#include "account/check"
#include "account/cmd"
#include "account/native"