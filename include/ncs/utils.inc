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