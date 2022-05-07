#define SKIN_RADIO_BLOCK_BLACK 0
#define SKIN_RADIO_BLOCK_WHITE 1

enum struct PlayerRadioBlock
{
	ArrayList BlackList;
	ArrayList WhiteList;
	int Mode;

	void Init(int mode, const char[] black, const char[] white)
	{
		if (!this.BlackList) this.BlackList = new ArrayList();
		if (!this.WhiteList) this.WhiteList = new ArrayList();
		this.BlackList.Clear();
		this.WhiteList.Clear();
		this.Mode = mode;

		static char buffer[MAX_SKINS_NUM][8];
		int length = ExplodeString(black, ",", buffer, sizeof(buffer), sizeof(buffer[]));
		for (int i = 0; i < length; i++) this.BlackList.Push(StringToInt(buffer[i]));
		length = ExplodeString(white, ",", buffer, sizeof(buffer), sizeof(buffer[]));
		for (int i = 0; i < length; i++) this.WhiteList.Push(StringToInt(buffer[i]));
	}
}

PlayerRadioBlock g_PlayerRadioBlock[MAXPLAYERS + 1];

char SkinRadioBlockModeSelection[][] = {
	"关闭",
	"仅自己",
	"黑名单",
	"白名单"
}

void SkinRadioBlockGetCookie(int client)
{
	static char buffer[8];
	char black[2048], white[2048];
	int mode;
	if (NCS_Cookie_Get(client, "skin_radio_block_mode", buffer, sizeof(buffer)))
		mode = StringToInt(buffer);
	NCS_Cookie_Get(client, "skin_radio_block_black", black, sizeof(black));
	NCS_Cookie_Get(client, "skin_radio_block_white", white, sizeof(white));
	g_PlayerRadioBlock[client].Init(mode, black, white);
}

static void ArrayListToString(ArrayList list, char[] buffer, int size)
{
	for (int i = 0; i < list.Length; i++)
		Format(buffer, size, "%s,%d", buffer, list.Get(i));
	strcopy(buffer, size, buffer[1]);
}

void SkinRadioBlockListSet(int client, int type, int id)
{
	char buffer[2048];
	ArrayList list;
	if (type == SKIN_RADIO_BLOCK_BLACK)
		list = g_PlayerRadioBlock[client].BlackList;
	else // SKIN_RADIO_BLOCK_WHITE
		list = g_PlayerRadioBlock[client].WhiteList;

	int index = list.FindValue(id);
	if (index == -1) list.Push(id);
	else list.Erase(index);

	ArrayListToString(list, buffer, sizeof(buffer));

	if (type == SKIN_RADIO_BLOCK_BLACK)
		NCS_Cookie_Set(client, "skin_radio_block_black", buffer);
	else // SKIN_RADIO_BLOCK_WHITE
		NCS_Cookie_Set(client, "skin_radio_block_white", buffer);
}

void SkinRadioBlockModeSet(int client)
{
	static char buffer[8];
	g_PlayerRadioBlock[client].Mode = (g_PlayerRadioBlock[client].Mode + 1) % sizeof(SkinRadioBlockModeSelection);
	IntToString(g_PlayerRadioBlock[client].Mode, buffer, sizeof(buffer));
	NCS_Cookie_Set(client, "skin_radio_block_mode", buffer);
}
