#define INT_MAX 2147483647
#define INT64_LENGTH 20

// Get day format(YYYYMMDD)
stock int GetDay(int today = 0)
{
	char time[32];
	FormatTime(time, 32, "%Y%m%d", !today ? GetTime() : today);
	return StringToInt(time);
}

// Convert Steam64 To SteamID
stock void Steam64toSteamID(const char[] friendId, char[] steamid, int iLen)
{
	char[] szBase = "76561197960265728";
	char szSteam[18], szAccount[18];
	int iBorrow, iY, iZ, iTemp;

	strcopy(szSteam, 18, friendId);

	if(CharToNumber(szSteam[16]) % 2 == 1)
	{
		iY = 1;
		szSteam[16] = NumberToChar(CharToNumber(szSteam[16]) - 1);
	}
	
	for(int k = 16; k >= 0; k--)
	{
		if(iBorrow > 0)
		{
			iTemp = CharToNumber(szSteam[k]) - 1;
			
			if(iTemp >= CharToNumber(szBase[k]))
			{
				iBorrow = 0;
				szAccount[k] = NumberToChar(iTemp - CharToNumber(szBase[k]));
			}
			else
			{
				iBorrow = 1;
				szAccount[k] = NumberToChar((iTemp + 10) - CharToNumber(szBase[k]));
			}
		}
		else
		{
			if(CharToNumber(szSteam[k]) >= CharToNumber(szBase[k]))
			{
				iBorrow = 0;
				szAccount[k] = NumberToChar(CharToNumber(szSteam[k]) - CharToNumber(szBase[k]));
			}
			else
			{
				iBorrow = 1;
				szAccount[k] = NumberToChar((CharToNumber(szSteam[k]) + 10) - CharToNumber(szBase[k]));
			}
		}
	}
	
	iZ = StringToInt(szAccount);
	iZ /= 2;
	
	FormatEx(steamid, iLen, "STEAM_1:%d:%d", iY, iZ);
}

stock int NumberToChar(const int iNum)
{
	return '0' + ((iNum >= 0 && iNum <= 9) ? iNum : 0);
}

stock int CharToNumber(const int cNum)
{
	return (cNum >= '0' && cNum <= '9') ? (cNum - '0') : 0;
}

// Get chinese time
stock void ChineseTime(char[] ctime, int size)
{
	char time[3];
	FormatTime(time, 3, "%H", GetTime());
	int i_time = StringToInt(time);
	if (i_time <= 4)
		Format(ctime, size, "凌晨");
	else if (i_time <= 6)
		Format(ctime, size, "清晨");
	else if (i_time <= 8)
		Format(ctime, size, "早上");
	else if (i_time <= 11)
		Format(ctime, size, "中午");
	else if (i_time <= 17)
		Format(ctime, size, "下午");
	else if (i_time <= 19)
		Format(ctime, size, "傍晚");
	else
		Format(ctime, size, "晚上");
}

stock void GenerateRandomString(char[] buffer, int maxLen)
{
	// terminator
	maxLen--;

	char random[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234556789";
	int randlen = strlen(random) - 1;

	int n = 0;
	int c = 0;

	while(n < maxLen)
	{
		if(random[0] == '\0')
		{
			c = GetRandomInt(33, 126);
			buffer[n] = c;
		}
		else
		{
			c = GetRandomInt(0, randlen);
			buffer[n] = random[c];
		}

		n++;
	}

	buffer[maxLen] = '\0';
}

// result out of range will return false
stock bool Int64Sum(int target[2], int add[2])
{
	if (INT_MAX - target[1] < add[1])
		return false;

	target[1] += add[1];

	int tmp = INT_MAX - target[0];
	if (tmp < add[0])
	{
		if (INT_MAX - target[1] == 0)
			return false;
		target[1]++;
		target[0] = add[0] - tmp;
	}
	else
	{
		target[0] += add[0];
	}
	return true;
}