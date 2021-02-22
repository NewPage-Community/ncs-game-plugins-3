#if defined _NCS_Color_
	#endinput
#endif
#define _NCS_Color_

#define MAX_COLOR_LENGTH 16

// Chat Color code

stock void ProcessColorString(char[] message, int maxLen, int team = 0)
{
	char game[32];
	GetGameFolderName(game, 32);

	if (!strcmp(game, "insurgency") || !strcmp(game, "doi"))
	{
		ReplaceString(message, maxLen, "{normal}",      "\x01", false);
		ReplaceString(message, maxLen, "{default}",     "\x01", false);
		ReplaceString(message, maxLen, "{white}",       "\x01", false);
		ReplaceString(message, maxLen, "{darkred}",     "\x088B0000FF", false);
		ReplaceString(message, maxLen, "{pink}",        "\x08FFC0CBFF", false);
		ReplaceString(message, maxLen, "{green}",       "\x083EFF3EFF", false);
		ReplaceString(message, maxLen, "{lime}",        "\x0800FF00FF", false);
		ReplaceString(message, maxLen, "{yellow}",      "\x08FFFF00FF", false);
		ReplaceString(message, maxLen, "{lightgreen}",  "\x0899FF99FF", false);
		ReplaceString(message, maxLen, "{lightred}",    "\x08FF4040FF", false);
		ReplaceString(message, maxLen, "{red}",         "\x08FF4040FF", false);
		ReplaceString(message, maxLen, "{gray}",        "\x08CCCCCCFF", false);
		ReplaceString(message, maxLen, "{grey}",        "\x08CCCCCCFF", false);
		ReplaceString(message, maxLen, "{olive}",       "\x089EC34FFF", false);
		ReplaceString(message, maxLen, "{orange}",      "\x08FFA500FF", false);
		ReplaceString(message, maxLen, "{silver}",      "\x08C0C0C0FF", false);
		ReplaceString(message, maxLen, "{lightblue}",   "\x08ADD8E6FF", false);
		ReplaceString(message, maxLen, "{blue}",        "\x0899CCFFFF", false);
		ReplaceString(message, maxLen, "{purple}",      "\x08800080FF", false);
		ReplaceString(message, maxLen, "{darkorange}",  "\x08FF8C00FF", false);
		switch(team)
		{
	  		case 3 : ReplaceString(message, maxLen, "{name}", "\x08AC4029FF", false);
	   		case 2 : ReplaceString(message, maxLen, "{name}", "\x0884961CFF", false);
			default: ReplaceString(message, maxLen, "{name}", "\x05", false);
		}
	}
	else if (!strcmp(game, "csgo"))
	{
		ReplaceString(message, maxLen, "{normal}",      "\x01", false);
		ReplaceString(message, maxLen, "{default}",     "\x01", false);
		ReplaceString(message, maxLen, "{white}",       "\x01", false);
		ReplaceString(message, maxLen, "{darkred}",     "\x02", false);
		ReplaceString(message, maxLen, "{pink}",        "\x03", false);
		ReplaceString(message, maxLen, "{green}",       "\x04", false);
		ReplaceString(message, maxLen, "{lime}",        "\x05", false);
		ReplaceString(message, maxLen, "{yellow}",      "\x05", false);
		ReplaceString(message, maxLen, "{lightgreen}",  "\x06", false);
		ReplaceString(message, maxLen, "{lightred}",    "\x07", false);
		ReplaceString(message, maxLen, "{red}",         "\x07", false);
		ReplaceString(message, maxLen, "{gray}",        "\x08", false);
		ReplaceString(message, maxLen, "{grey}",        "\x08", false);
		ReplaceString(message, maxLen, "{olive}",       "\x09", false);
		ReplaceString(message, maxLen, "{orange}",      "\x10", false);
		ReplaceString(message, maxLen, "{silver}",      "\x0A", false);
		ReplaceString(message, maxLen, "{lightblue}",   "\x0B", false);
		ReplaceString(message, maxLen, "{blue}",        "\x0C", false);
		ReplaceString(message, maxLen, "{purple}",      "\x0E", false);
		ReplaceString(message, maxLen, "{darkorange}",  "\x0F", false);
		switch(team)
		{
	  		case 3 : ReplaceString(message, maxLen, "{name}", "\x0B", false);
	   		case 2 : ReplaceString(message, maxLen, "{name}", "\x05", false);
			default: ReplaceString(message, maxLen, "{name}", "\x01", false);
		}
	}
	else if (!strcmp(game, "cstrike"))
	{
		ReplaceString(message, maxLen, "{normal}",      "\x01", false);
		ReplaceString(message, maxLen, "{default}",     "\x01", false);
		ReplaceString(message, maxLen, "{white}",       "\x07FFFFFF", false);
		ReplaceString(message, maxLen, "{darkred}",     "\x078B0000", false);
		ReplaceString(message, maxLen, "{pink}",        "\x07FF1493", false);
		ReplaceString(message, maxLen, "{green}",       "\x073EFF3E", false);
		ReplaceString(message, maxLen, "{lime}",        "\x0700FF00", false);
		ReplaceString(message, maxLen, "{yellow}",      "\x07FFFF00", false);
		ReplaceString(message, maxLen, "{lightgreen}",  "\x0799FF99", false);
		//ReplaceString(message, maxLen, "{lightred}",    "\x07", false);
		ReplaceString(message, maxLen, "{red}",         "\x07FF4040", false);
		ReplaceString(message, maxLen, "{gray}",        "\x07CCCCCC", false);
		ReplaceString(message, maxLen, "{grey}",        "\x07CCCCCC", false);
		ReplaceString(message, maxLen, "{olive}",       "\x079EC34F", false);
		ReplaceString(message, maxLen, "{orange}",      "\x07FFA500", false);
		ReplaceString(message, maxLen, "{silver}",      "\x07C0C0C0", false);
		ReplaceString(message, maxLen, "{lightblue}",   "\x07ADD8E6", false);
		ReplaceString(message, maxLen, "{blue}",        "\x0799CCFF", false);
		ReplaceString(message, maxLen, "{purple}",      "\x07800080", false);
		ReplaceString(message, maxLen, "{darkorange}",  "\x07FF8C00", false);
		ReplaceString(message, maxLen, "{name}", 		"\x03", false);
	}
	else
	{
		ReplaceString(message, maxLen, "{normal}",      "\x01", false);
		ReplaceString(message, maxLen, "{default}",     "\x01", false);
		ReplaceString(message, maxLen, "{white}",       "\x01", false);
		ReplaceString(message, maxLen, "{darkred}",     "\x02", false);
		ReplaceString(message, maxLen, "{pink}",        "\x03", false);
		ReplaceString(message, maxLen, "{green}",       "\x04", false);
		ReplaceString(message, maxLen, "{lime}",        "\x05", false);
		ReplaceString(message, maxLen, "{yellow}",      "\x05", false);
		ReplaceString(message, maxLen, "{lightgreen}",  "\x06", false);
		ReplaceString(message, maxLen, "{lightred}",    "\x07", false);
		ReplaceString(message, maxLen, "{red}",         "\x07", false);
		ReplaceString(message, maxLen, "{gray}",        "\x08", false);
		ReplaceString(message, maxLen, "{grey}",        "\x08", false);
		ReplaceString(message, maxLen, "{olive}",       "\x09", false);
		ReplaceString(message, maxLen, "{orange}",      "\x10", false);
		ReplaceString(message, maxLen, "{silver}",      "\x0A", false);
		ReplaceString(message, maxLen, "{lightblue}",   "\x0B", false);
		ReplaceString(message, maxLen, "{blue}",        "\x0C", false);
		ReplaceString(message, maxLen, "{purple}",      "\x0E", false);
		ReplaceString(message, maxLen, "{darkorange}",  "\x0F", false);
		ReplaceString(message, maxLen, "{name}", 		"\x01", false);
	}
}

stock void RemoveColorString(char[] message, int maxLen)
{
	ReplaceString(message, maxLen, "{normal}",      "", false);
	ReplaceString(message, maxLen, "{default}",     "", false);
	ReplaceString(message, maxLen, "{white}",       "", false);
	ReplaceString(message, maxLen, "{darkred}",     "", false);
	ReplaceString(message, maxLen, "{pink}",        "", false);
	ReplaceString(message, maxLen, "{green}",       "", false);
	ReplaceString(message, maxLen, "{lime}",        "", false);
	ReplaceString(message, maxLen, "{yellow}",      "", false);
	ReplaceString(message, maxLen, "{lightgreen}",  "", false);
	ReplaceString(message, maxLen, "{lightred}",    "", false);
	ReplaceString(message, maxLen, "{red}",         "", false);
	ReplaceString(message, maxLen, "{gray}",        "", false);
	ReplaceString(message, maxLen, "{grey}",        "", false);
	ReplaceString(message, maxLen, "{olive}",       "", false);
	ReplaceString(message, maxLen, "{orange}",      "", false);
	ReplaceString(message, maxLen, "{silver}",      "", false);
	ReplaceString(message, maxLen, "{lightblue}",   "", false);
	ReplaceString(message, maxLen, "{blue}",        "", false);
	ReplaceString(message, maxLen, "{purple}",      "", false);
	ReplaceString(message, maxLen, "{darkorange}",  "", false);
	ReplaceString(message, maxLen, "{name}",        "", false);
	ReplaceString(message, maxLen, "\x01",          "", false);
	ReplaceString(message, maxLen, "\x02",          "", false);
	ReplaceString(message, maxLen, "\x03",          "", false);
	ReplaceString(message, maxLen, "\x04",          "", false);
	ReplaceString(message, maxLen, "\x05",          "", false);
	ReplaceString(message, maxLen, "\x06",          "", false);
	ReplaceString(message, maxLen, "\x07",          "", false);
	ReplaceString(message, maxLen, "\x08",          "", false);
	ReplaceString(message, maxLen, "\x09",          "", false);
	ReplaceString(message, maxLen, "\x10",          "", false);
	ReplaceString(message, maxLen, "\x0A",          "", false);
	ReplaceString(message, maxLen, "\x0B",          "", false);
	ReplaceString(message, maxLen, "\x0C",          "", false);
	ReplaceString(message, maxLen, "\x0D",          "", false);
	ReplaceString(message, maxLen, "\x0E",          "", false);
	ReplaceString(message, maxLen, "\x0F",          "", false);
}

// Color chat

stock void CPrintToChat(int client, const char[] format, any...)
{
	if (!IsClientInGame(client))
		return;
	
	char buffer[254];
	SetGlobalTransTarget(client);
	VFormat(buffer, sizeof(buffer), format, 3);
	ProcessColorString(buffer, sizeof(buffer));
	PrintToChat(client, "%s", buffer);
}

stock void CPrintToChatAll(const char[] format, any...)
{
	char buffer[254];
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			SetGlobalTransTarget(i);
			VFormat(buffer, sizeof(buffer), format, 2);
			ProcessColorString(buffer, sizeof(buffer));
			PrintToChat(i, "%s", buffer);
		}
	}
}