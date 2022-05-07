#define COOKIE_TITLE_TYPE_KEY "title_type"
#define COOKIE_TITLE_CUSTOM_KEY "title_custom"

int GetCookieTitleType(int client)
{
	char type[8];
	NCS_Cookie_Get(client, COOKIE_TITLE_TYPE_KEY, type, sizeof(type));
	return StringToInt(type);
}

void SetCookieTitleType(int client, int num)
{
	char type[8];
	IntToString(num, type, sizeof(type));
	NCS_Cookie_Set(client, COOKIE_TITLE_TYPE_KEY, type);
}

void GetCookieTitleCustom(int client, char[] buffer, int maxLen)
{
	NCS_Cookie_Get(client, COOKIE_TITLE_CUSTOM_KEY, buffer, maxLen);
}

void SetCookieTitleCustom(int client, const char[] buffer)
{
	NCS_Cookie_Set(client, COOKIE_TITLE_CUSTOM_KEY, buffer);
}