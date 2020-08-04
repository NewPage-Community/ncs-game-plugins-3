#define MAX_TITLE_LENGTH 32

enum struct PlayerInfo
{
    int index;
    char title[MAX_TITLE_LENGTH];
    char titleColor[MAX_COLOR_LENGTH];
    char nameColor[MAX_COLOR_LENGTH];

    bool IsValid()
    {
		if (this.index == 0)
			return false;
		return IsValidClient(this.index);
	}

    void GetChatName(char[] buffer, int size)
    {
        NCS_Account_GetName(this.index, buffer, size);
        // Default name color
        if (this.nameColor[0] == '\0')
            this.nameColor = "{name}";
        if (this.title[0] == '\0')
            Format(buffer, size, "%s%s", this.nameColor, buffer);
        else
            Format(buffer, size, "%s%s %s%s", this.titleColor, this.title, this.nameColor, buffer);
    }

    void SetTitle(const char[] title, const char[] titleColor)
    {
        strcopy(this.title, MAX_TITLE_LENGTH, title);
        strcopy(this.titleColor, MAX_COLOR_LENGTH, titleColor);
    }

    void SetNameColor(const char[] nameColor)
    {
        strcopy(this.nameColor, MAX_COLOR_LENGTH, nameColor);
    }

    void Clear()
    {
        this.index = 0;
        this.title[0] = '\0';
        this.titleColor[0] = '\0';
        this.nameColor[0] = '\0';
    }
}

PlayerInfo player[MAXPLAYERS + 1];

#include "chat/basechat"
#include "chat/native"