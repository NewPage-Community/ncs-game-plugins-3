#define MAX_STEAMID_LENGTH INT64_LENGTH
#define MAX_UID_LENGTH INT64_LENGTH
#define MAX_GID_LENGTH INT64_LENGTH

enum struct account
{
	int index;
	char uid[MAX_UID_LENGTH];
	char steamid[MAX_STEAMID_LENGTH];
	char name[MAX_NAME_LENGTH];
	ArrayList groups;

	bool Valid() {
		if (this.index == 0 || this.uid[0] == '\0')
			return false;
		char steamid[MAX_STEAMID_LENGTH];
		if (!GetClientAuthId(this.index, AuthId_SteamID64, steamid, sizeof(steamid)))
			return false;
		return IsValidClient(this.index) && StrEqual(steamid, this.steamid)
	}

	void Init(int index, const char[] uid, const char[] steamid) {
		this.index = index;
		strcopy(this.uid, MAX_UID_LENGTH, uid);
		strcopy(this.steamid, MAX_STEAMID_LENGTH, steamid);
		GetClientName(index, this.name, MAX_NAME_LENGTH);
		// avoid color tag hack
		RemoveColorString(this.name, MAX_NAME_LENGTH);
		this.groups = new ArrayList(MAX_GID_LENGTH);
	}

	void UpdateName(const char[] username) {
		if (!this.Valid())
			return;
		strcopy(this.name, MAX_STEAMID_LENGTH, username);
		// avoid color tag hack
		RemoveColorString(this.name, MAX_NAME_LENGTH);
	}

	void Clean() {
		this.index = 0;
		this.uid[0] = '\0';
		this.steamid[0] = '\0';
		this.name[0] = '\0';
		delete this.groups;
	}
}

account g_Accounts[MAXPLAYERS + 1];

#include "account/name"
#include "account/uid"
#include "account/api"
#include "account/check"
#include "account/cmd"
#include "account/native"
#include "account/menu"