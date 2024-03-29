#define PROGRESS_LENGTH 20
#define LEVEL_POINT 10800
#define MAX_LEVEL 31
#define MAX_ITEM_NAME_LENGTH 64

ConVar cv_pass_starttime;
ConVar cv_pass_endtime;

void InitConVar()
{
	cv_pass_starttime = CreateConVar("pass_starttime", "0", "", 0, true, 0.0);
	cv_pass_endtime = CreateConVar("pass_endtime", "0", "", 0, true, 0.0);
}

enum struct PassInfo
{
	char uid[MAX_UID_LENGTH];
	int index;
	int point;
	int type;
	int box_amount;

	bool IsValid()
	{
		if (this.index == 0 || this.uid[0] == '\0')
			return false;
		return IsValidClient(this.index);
	}

	void Init(int index, const char[] uid)
	{
		this.index = index;
		strcopy(this.uid, MAX_UID_LENGTH, uid);
		this.point = 0;
	}

	void Clear()
	{
		this.uid[0] = '\0';
		this.index = 0;
		this.point = 0;
	}

	void AddPoint(int point)
	{
		if (IsPassEnabled())
			this.point += point;
	}

	void PushJSONArray(JSONArray array)
	{
		JSONObject pass = new JSONObject();
		pass.SetInt64("uid", this.uid);
		pass.SetInt("point", this.point);
		array.Push(pass);
		delete pass;
	}
}

enum RewardType
{
	FreeReward = 0,
	AdvReward
}

enum struct Reward
{
	int id;
	int level;
	char name[MAX_ITEM_NAME_LENGTH];
	int amount;
	int length;

	void GetName(char[] buffer, int size)
	{
		if (this.amount == 0 && this.length == 0)
			Format(buffer, size, "%s", this.name);
		else if (this.amount != 0)
			Format(buffer, size, "%s(%d个)", this.name, this.amount);
		else if (this.length != 0)
			Format(buffer, size, "%s(%d天)", this.name, this.length / 86400);
	}

	bool IsValid()
	{
		return this.name[0] != '\0';
	}
}

void ProgressBar(int point, char[] buffer, int size)
{
	int left = (point % LEVEL_POINT) * PROGRESS_LENGTH / LEVEL_POINT;
	for (int i = 0; i < PROGRESS_LENGTH; i++)
	{
		if (left >= (i + 1))
			Format(buffer, size, "%s#", buffer);
		else
			Format(buffer, size, "%s. ", buffer);
	}
	Format(buffer, size, "[%s]", buffer);
}

int Level(int point)
{
	if (point == 0)
		return 0;
	return point / LEVEL_POINT + 1;
}

bool IsPassEnabled()
{
	int now = GetTime();
	int start = cv_pass_starttime.IntValue;
	int end = cv_pass_endtime.IntValue;
	return start <= now && now < end
}

Reward rewards[MAX_LEVEL][2];

PassInfo playerPass[MAXPLAYERS+1];

char passTypeName[][] = {"免费通行证", "高级通行证", "终极通行证"};

int season;

#include "pass/api"
#include "pass/cache"
#include "pass/player"
#include "pass/native"
#include "pass/menu"
#include "pass/reward"
#include "pass/cmd"
