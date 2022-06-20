#define Backpack_Service "backpack/user"

int playerLast[MAXPLAYERS + 1];
int playerLastSkin[MAXPLAYERS + 1];

void SkinMainMenu(int client)
{
	int skinID = g_UsersUsedSkin[client];
	Skin skin;
	if (skinID) g_Skins.Get(skinID, skin);
	int arm = GetUserSkinArm(client);
	int sound = GetUserSkinSound(client);

	MenuEx menu = new MenuEx(SkinMainMenuHandler);
	menu.SetTitle("皮肤系统");
	menu.AddItemEx(ITEMDRAW_DEFAULT, "skin", "装备皮肤: %s", skinID ? skin.name : "默认皮肤");
	if (arm == -1)
	{
		menu.AddItemEx(ITEMDRAW_DEFAULT, "arm", "装备手模: 跟随皮肤");
	}
	else if (arm == 0)
	{
		menu.AddItemEx(ITEMDRAW_DEFAULT, "arm", "装备手模: 原版");
	}
	else
	{
		g_Skins.Get(arm, skin);
		menu.AddItemEx(ITEMDRAW_DEFAULT, "arm", "装备手模: 自定义");
		menu.AddItemEx(ITEMDRAW_DEFAULT, "arm_custom", "自定义手模: %s", skin.name);
	}
	if (sound == -1)
	{
		menu.AddItemEx(ITEMDRAW_DEFAULT, "sound", "装备语音: 跟随皮肤");
	}
	else if (sound == 0)
	{
		menu.AddItemEx(ITEMDRAW_DEFAULT, "sound", "装备语音: 原版");
	}
	else
	{
		g_Skins.Get(sound, skin);
		menu.AddItemEx(ITEMDRAW_DEFAULT, "sound", "装备语音: 自定义");
		menu.AddItemEx(ITEMDRAW_DEFAULT, "sound_custom", "自定义语音: %s", skin.name);
	}
	menu.AddItem("sound_settings", "语音设置");
	menu.Display(client, MENU_TIME_FOREVER);
}

public int SkinMainMenuHandler(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_End) delete menu;
	else if (action == MenuAction_Select)
	{
		static char item[20];
		menu.GetItem(slot, item, sizeof(item));
		if (!strcmp(item, "skin"))
		{
			DisplayUserSkins(client);
		}
		else if (!strcmp(item, "arm"))
		{
			int arm = GetUserSkinArm(client);
			if (arm == -1) arm = 0;
			else if (arm == 0) arm = g_UsersUsedSkin[client];
			else arm = -1;
			SetUserSkinArm(client, arm);
			SkinMainMenu(client);
		}
		else if (!strcmp(item, "sound"))
		{
			int sound = GetUserSkinSound(client);
			if (sound == -1) sound = 0;
			else if (sound == 0) sound = g_UsersUsedSkin[client];
			else sound = -1;
			SetUserSkinSound(client, sound);
			SkinMainMenu(client);
		}
		else if (!strcmp(item, "arm_custom"))
		{
			SkinArmCustomMenu(client);
		}
		else if (!strcmp(item, "sound_custom"))
		{
			SkinSoundCustomMenu(client);
		}
		else if (!strcmp(item, "sound_settings"))
		{
			SkinRadioMenu(client);
		}
	}
	return 0;
}

#define SKIN_SKINMENU(%0,%1) \
void %0(int client) \
{ \
	static char uid[MAX_UID_LENGTH]; \
	if (!NCS_Account_GetUID(client, uid, sizeof(uid))) \
	{ \
		NCS_Chat(client, _, "您的用户数据尚未加载，请重试"); \
		return; \
	} \
	DataPack dp = new DataPack(); \
	dp.WriteCell(client, false); \
	dp.WriteString(uid, false); \
	static char path[128]; \
	Format(path, sizeof(path), "%s/%s", Backpack_Service, uid); \
	NewNCSAPI(path).Get(%1, dp); \
}

SKIN_SKINMENU(DisplayUserSkins, UserSkinsCallback)
SKIN_SKINMENU(SkinArmCustomMenu, SkinArmCustomMenuCallback)
SKIN_SKINMENU(SkinSoundCustomMenu, SkinSoundCustomMenuCallback)

#define SKIN_SKINMENU_CALLBACK(%0,%1,%2) \
public void %0(HTTPResponse response, DataPack dp, const char[] error) \
{ \
	int type = %2; \
	if (CheckAPIError("Skin", "UserSkinsCallback", response, error)) \
		return; \
	dp.Reset(); \
	int client = dp.ReadCell(); \
	char uid[MAX_UID_LENGTH]; \
	dp.ReadString(uid, sizeof(uid)); \
	dp.Close(); \
	char iUID[MAX_UID_LENGTH]; \
	JSONObject data = view_as<JSONObject>(response.Data); \
	JSONArray items = view_as<JSONArray>(data.Get("items")); \
	int length = items.Length; \
	data.GetString("uid", iUID, sizeof(iUID)); \
	if (!StrEqual(iUID, uid) || !IsValidClient(client)) \
	{ \
		delete items; \
		return; \
	} \
	Menu menu = new Menu(%1); \
	menu.ExitBackButton = true; \
	if (type == 0) menu.SetTitle("皮肤系统 - 装备皮肤"); \
	if (type == 1) menu.SetTitle("皮肤系统 - 自定义手模"); \
	if (type == 2) menu.SetTitle("皮肤系统 - 自定义语音"); \
	if (type == 0) menu.AddItem("0", "默认皮肤"); \
	int mySkinCount; \
	int usedSkin = g_UsersUsedSkin[client]; \
	for (int i = 0; i < length; i++) \
	{ \
		JSONObject item = view_as<JSONObject>(items.Get(i)); \
		char skinID[8]; \
		char exprTime[INT64_LENGTH]; \
		IntToString(item.GetInt("id"), skinID, sizeof(skinID)); \
		item.GetString("expr_time", exprTime, sizeof(exprTime)); \
		Skin s; \
		if (!g_Skins.Get(item.GetInt("id"), s)) \
		{ \
			delete item; \
			continue; \
		} \
		delete item; \
		if (type == 1 && !s.armPath[0]) continue; \
		if (type == 2 && !s.sounds.Size) continue; \
		char desc[64]; \
		int left = StringToInt(exprTime) - GetTime(); \
		if (left > 0) \
			Format(desc, sizeof(desc), "%s(%d天%d小时)", s.name, left / 86400, (left % 86400) / 3600); \
		else \
			Format(desc, sizeof(desc), "%s", s.name); \
		bool used = usedSkin == s.id; \
		if (used) \
			Format(desc, sizeof(desc), "(使用中)%s", desc); \
		menu.AddItem(skinID, desc); \
		if (used) mySkinCount = menu.ItemCount - 1; \
	} \
	AddUserGroupSkinMenu(menu, client, mySkinCount); \
	if (playerLast[client]) menu.DisplayAt(client, playerLast[client], MENU_TIME_FOREVER); \
	else if (mySkinCount) menu.DisplayAt(client, mySkinCount - (mySkinCount % menu.Pagination), MENU_TIME_FOREVER); \
	else menu.Display(client, MENU_TIME_FOREVER); \
	delete items; \
}

SKIN_SKINMENU_CALLBACK(UserSkinsCallback, Menu_SkinSelected, 0)
SKIN_SKINMENU_CALLBACK(SkinArmCustomMenuCallback, Menu_ArmSelected, 1)
SKIN_SKINMENU_CALLBACK(SkinSoundCustomMenuCallback, Menu_SoundSelected, 2)

public int Menu_SkinSelected(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (slot == MenuCancel_ExitBack) SkinMainMenu(client);
	else if (action == MenuAction_Select)
	{
		if (slot == MenuEnd_Exit) return 0;
		if (IsValidClient(client))
		{
			if (slot == 0) {
				SetUsedSkin(client, 0);
				return 0;
			}
			playerLast[client] = menu.Selection;
			char skinID[32];
			menu.GetItem(slot, skinID, sizeof(skinID));
			DisplaySkinMenu(client, StringToInt(skinID));
		}
	}
	return 0;
}

public int Menu_ArmSelected(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		if (slot == MenuEnd_Exit) return 0;
		if (IsValidClient(client))
		{
			char skinID[32];
			menu.GetItem(slot, skinID, sizeof(skinID));
			SetUserSkinArm(client, StringToInt(skinID));
			SetPlayerModelExt(client, g_UsersUsedSkin[client]);
		}
		SkinMainMenu(client);
	}
	return 0;
}

public int Menu_SoundSelected(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		if (slot == MenuEnd_Exit) return 0;
		if (IsValidClient(client))
		{
			char skinID[32];
			menu.GetItem(slot, skinID, sizeof(skinID));
			SetUserSkinSound(client, StringToInt(skinID));
		}
		SkinMainMenu(client);
	}
	return 0;
}

void DisplaySkinMenu(int client, int skinID)
{
	if (!skinID) return;
	playerLastSkin[client] = skinID;

	static Skin skin;
	if (!g_Skins.Get(skinID, skin)) return;

	bool used = g_UsersUsedSkin[client] == skin.id;
	MenuEx menu = new MenuEx(Menu_Skin);
	menu.SetTitle("%s - 皮肤设置\n", skin.name);
	menu.AddItemEx(used ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT, "", "装备皮肤%s", used ? "(使用中)" : "");
	menu.AddItem("", "组件设置", !skin.bodys.Length ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);
	menu.AddItem("", "服装设置", !skin.skins.Length ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

public int Menu_Skin(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_End) delete menu;
	else if (slot == MenuCancel_ExitBack) DisplayUserSkins(client);
	else if (action == MenuAction_Select)
	{
		if (IsValidClient(client))
		{
			int skinID = playerLastSkin[client];
			if (slot == 0)
			{
				static Skin skin;
				if (!g_Skins.Get(skinID, skin)) return 0;
				SetUsedSkin(client, skinID);
#if defined GAME_INSURGENCY
				SetPlayerModelExt(client, skinID, true, true);
#else
				SetModel(client);
#endif
				NCS_Chat(client, _, "已成功更换为 {lime}%s{default}，可通过 {olive}!tp{default} 查看模型", skin.name);
			}
			else if (slot == 1)
			{
				SkinBodyMenu(client, skinID);
			}
			else if (slot == 2)
			{
				SkinSkinMenu(client, skinID);
			}
		}
	}
	return 0;
}

void SkinBodyMenu(int client, int skinID, int selection = 0)
{
	static BodysData body;
	Skin skin;
	if (!g_Skins.Get(skinID, skin)) return;

	Menu menu = CreateMenu(SkinBodyMenuHandler);
	menu.SetTitle("%s - 组件设置", skin.name);

	static char info[128];
	menu.AddItem("", "重置");
	for (int i = 0; i < skin.bodys.Length; i++) {
		if (!skin.bodys.GetArray(i, body)) ThrowError("GetArray");
		int value;
#if defined GAME_INSURGENCY
		CBaseAnimating animating = CBaseAnimating(client);
		value = animating.GetBodyGroup(GetEntProp(client, Prop_Send, "m_nBody"), body.index);
#endif
		Format(info, sizeof(info), "%s: 形态%d", body.name, value + 1);
		menu.AddItem("", info);
	}
	menu.ExitBackButton = true;
	if (selection) menu.DisplayAt(client, selection, MENU_TIME_FOREVER);
	else menu.Display(client, MENU_TIME_FOREVER);
}

int SkinBodyMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) delete menu;
	else if (param == MenuCancel_ExitBack) DisplayUserSkins(client);
	else if (action == MenuAction_Select)
	{
		if (param == MenuCancel_ExitBack)
		{
			DisplayUserSkins(client);
			return 0;
		}

		static Skin skin;
		if (!g_Skins.Get(playerLastSkin[client], skin)) return 0;
		if (param == 0)
		{
			NCS_Chat(client, _, "组件已重置");
			SetUserSkinBody(client, playerLastSkin[client], skin.initBody);
#if defined GAME_INSURGENCY
			SetPlayerModelExt(client, playerLastSkin[client], true);
#endif
		}
		else
		{
			static BodysData body;
			if (!skin.bodys.GetArray(param - 1, body)) ThrowError("GetArray");
			int value;
#if defined GAME_INSURGENCY
			CBaseAnimating animating = CBaseAnimating(client);
			value = animating.GetBodyGroup(GetEntProp(client, Prop_Send, "m_nBody"), body.index) + 1;
			value %= animating.GetBodyGroupCount(body.index);
			animating.SetBodyGroup(body.index, value);
			SetUserSkinBody(client, playerLastSkin[client], GetEntProp(client, Prop_Send, "m_nBody"));
			SetPlayerModelExt(client, playerLastSkin[client], true);
#endif
			NCS_Chat(client, _, "组件 {lime}%s{default} 已设置为 {lime}形态%d", body.name, value + 1);
			if (GetUserFlagBits(client) & ADMFLAG_ROOT)
				PrintToConsole(client, "[BodyGroup] %s: %d", skin.uid, GetEntProp(client, Prop_Send, "m_nBody"));
		}
		SkinBodyMenu(client, playerLastSkin[client], menu.Selection);
	}
	return 0;
}

void SkinRadioMenu(int client)
{
	int skinID = g_UsersUsedSkin[client];
	Skin skin;
	if (skinID) g_Skins.Get(skinID, skin);

	MenuEx menu = new MenuEx(SkinRadioMenuHandler);
	menu.ExitBackButton = true;
	menu.SetTitle("皮肤系统 - 语音设置");
	menu.AddItemEx(ITEMDRAW_DEFAULT, "", "屏蔽模式: %s", SkinRadioBlockModeSelection[g_PlayerRadioBlock[client].Mode]);
	menu.AddItem("", "黑名单设置");
	menu.AddItem("", "白名单设置");
	menu.AddItemEx(ITEMDRAW_DEFAULT, "", "聆听自己音量: %d%", g_PlayerRadioVolume[client][SKIN_RADIO_VOL_MYSELF]);
	menu.AddItemEx(ITEMDRAW_DEFAULT, "", "聆听他人音量: %d%", g_PlayerRadioVolume[client][SKIN_RADIO_VOL_OTHER]);
	menu.Display(client, MENU_TIME_FOREVER);
}

public int SkinRadioMenuHandler(Menu menu, MenuAction action, int client, int slot)
{
	if (action == MenuAction_End) delete menu;
	else if (slot == MenuCancel_ExitBack) SkinMainMenu(client);
	else if (action == MenuAction_Select)
	{
		if (slot == 0)
		{
			SkinRadioBlockModeSet(client);
			SkinRadioMenu(client);
		}
		else if (slot == 1) SkinRadioListMenu(client, SKIN_RADIO_BLOCK_BLACK);
		else if (slot == 2) SkinRadioListMenu(client, SKIN_RADIO_BLOCK_WHITE);
		else if (slot == 3) SkinRadioVolumeMenu(client, SKIN_RADIO_VOL_MYSELF);
		else if (slot == 4) SkinRadioVolumeMenu(client, SKIN_RADIO_VOL_OTHER);
	}
	return 0;
}

void SkinRadioListMenu(int client, int type, int slot = 0)
{
	MenuEx menu;
	if (type == SKIN_RADIO_BLOCK_BLACK)
	{
		menu = new MenuEx(SkinRadioListMenuBlackHandler);
		menu.SetTitle("皮肤系统 - 语音黑名单");
	}
	else // SKIN_RADIO_BLOCK_WHITE
	{
		menu = new MenuEx(SkinRadioListMenuWhiteHandler);
		menu.SetTitle("皮肤系统 - 语音白名单");
	}
	menu.ExitBackButton = true;

	static Skin skin;
	static char buffer[128];
	StringMapSnapshot snapshot = g_Skins.Snapshot();
	for (int i = 0; i < snapshot.Length; i++)
	{
		snapshot.GetKey(i, buffer, sizeof(buffer));
		g_Skins.GetArray(buffer, skin, sizeof(skin));

		// Buffer -> id
		ArrayList list;
		if (type == SKIN_RADIO_BLOCK_BLACK)
			list = g_PlayerRadioBlock[client].BlackList
		else // SKIN_RADIO_BLOCK_WHITE
			list = g_PlayerRadioBlock[client].WhiteList;
		IntToString(skin.id, buffer, sizeof(buffer));
		if (list.FindValue(skin.id) != -1)
			menu.AddItemEx(ITEMDRAW_DEFAULT, buffer, "[%s]", skin.name);
		else
			menu.AddItem(buffer, skin.name);
	}
	delete snapshot;

	menu.DisplayAt(client, slot, MENU_TIME_FOREVER);
}

// %0 function %1 type
#define SKIN_RADIOLIST_HANDLER(%0,%1) \
public int %0(Menu menu, MenuAction action, int client, int slot) \
{ \
	if (action == MenuAction_End) delete menu; \
	else if (slot == MenuCancel_ExitBack) SkinRadioMenu(client); \
	else if (action == MenuAction_Select) \
	{ \
		static char buffer[8]; \
		menu.GetItem(slot, buffer, sizeof(buffer)); \
		SkinRadioBlockListSet(client, %1, StringToInt(buffer)); \
		SkinRadioListMenu(client, %1, menu.Selection); \
	} \
	return 0; \
}

SKIN_RADIOLIST_HANDLER(SkinRadioListMenuBlackHandler, SKIN_RADIO_BLOCK_BLACK)
SKIN_RADIOLIST_HANDLER(SkinRadioListMenuWhiteHandler, SKIN_RADIO_BLOCK_WHITE)

void SkinRadioVolumeMenu(int client, int type)
{
	int volume = g_PlayerRadioVolume[client][type];
	Menu menu;
	if (type == SKIN_RADIO_VOL_MYSELF)
	{
		menu = new Menu(SkinRadioVolumeMenuMyselfHandler);
		menu.SetTitle("皮肤系统 - 聆听自己音量\n当前音量: %d%", volume);
	}
	else // SKIN_RADIO_VOL_OTHER
	{
		menu = new Menu(SkinRadioVolumeMenuOtherHandler);
		menu.SetTitle("皮肤系统 - 聆听他人音量\n当前音量: %d%", volume);
	}
	menu.ExitBackButton = true;
	menu.AddItem("", "-10", volume > 0 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("", "-5", volume > 0 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("", "-1", volume > 0 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("", "+1", volume < 100 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("", "+5", volume < 100 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("", "+10", volume < 100 ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.Display(client, MENU_TIME_FOREVER);
}


// %0 function %1 type
#define SKIN_RADIOVOLUME_HANDLER(%0,%1) \
public int %0(Menu menu, MenuAction action, int client, int slot) \
{ \
	if (action == MenuAction_End) delete menu; \
	else if (slot == MenuCancel_ExitBack) SkinRadioMenu(client); \
	else if (action == MenuAction_Select) \
	{ \
		int volume = SkinRadioVolumeGet(client, %1); \
		if (slot == 0) volume = Max(0, volume - 10); \
		else if (slot == 1) volume = Max(0, volume - 5); \
		else if (slot == 2) volume = Max(0, volume - 1); \
		else if (slot == 3) volume = Min(100, volume + 1); \
		else if (slot == 4) volume = Min(100, volume + 5); \
		else if (slot == 5) volume = Min(100, volume + 10); \
		SkinRadioVolumeSet(client, %1, volume); \
		SkinRadioVolumeMenu(client, %1); \
	} \
	return 0; \
}

SKIN_RADIOVOLUME_HANDLER(SkinRadioVolumeMenuMyselfHandler, SKIN_RADIO_VOL_MYSELF)
SKIN_RADIOVOLUME_HANDLER(SkinRadioVolumeMenuOtherHandler, SKIN_RADIO_VOL_OTHER)

void SkinSkinMenu(int client, int skinID, int selection = 0)
{
	static BodysData body;
	Skin skin;
	if (!g_Skins.Get(skinID, skin)) return;

	Menu menu = CreateMenu(SkinSkinMenuHandler);
	menu.SetTitle("%s - 服装设置", skin.name);

	int index = GetUserSkinSkin(client, skinID);
	if (index == -1) index = 0;

	for (int i = 0; i < skin.skins.Length; i++) {
		if (!skin.skins.GetArray(i, body)) ThrowError("GetArray");
		menu.AddItem("", body.name, index != body.index ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	}
	menu.ExitBackButton = true;
	if (selection) menu.DisplayAt(client, selection, MENU_TIME_FOREVER);
	else menu.Display(client, MENU_TIME_FOREVER);
}

int SkinSkinMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) delete menu;
	else if (param == MenuCancel_ExitBack) DisplayUserSkins(client);
	else if (action == MenuAction_Select)
	{
		if (param == MenuCancel_ExitBack)
		{
			DisplayUserSkins(client);
			return 0;
		}

		static Skin skin;
		static BodysData body;
		if (!g_Skins.Get(playerLastSkin[client], skin)) ThrowError("GetSkin");
		if (!skin.skins.GetArray(param, body)) ThrowError("GetArray");
		SetUserSkinSkin(client, playerLastSkin[client], body.index);
#if defined GAME_INSURGENCY
		SetPlayerModelExt(client, playerLastSkin[client], true);
#else
		SetModel(client);
		SetEntProp(client, Prop_Send, "m_nSkin", body.index);
#endif
		NCS_Chat(client, _, "服装已设置为 {lime}%s", body.name);
		if (GetUserFlagBits(client) & ADMFLAG_ROOT)
			PrintToConsole(client, "[TextureGroup] %s: %d", skin.uid, GetEntProp(client, Prop_Send, "m_nSkin"));
		SkinSkinMenu(client, playerLastSkin[client], menu.Selection);
	}
	return 0;
}

// %0 function %1 type
#define SKIN_RADIOVOLUME_HANDLER(%0,%1) \
public int %0(Menu menu, MenuAction action, int client, int slot) \
{ \
	if (action == MenuAction_End) delete menu; \
	else if (slot == MenuCancel_ExitBack) SkinRadioMenu(client); \
	else if (action == MenuAction_Select) \
	{ \
		int volume = SkinRadioVolumeGet(client, %1); \
		if (slot == 0) volume = Max(0, volume - 10); \
		else if (slot == 1) volume = Max(0, volume - 5); \
		else if (slot == 2) volume = Max(0, volume - 1); \
		else if (slot == 3) volume = Min(100, volume + 1); \
		else if (slot == 4) volume = Min(100, volume + 5); \
		else if (slot == 5) volume = Min(100, volume + 10); \
		SkinRadioVolumeSet(client, %1, volume); \
		SkinRadioVolumeMenu(client, %1); \
	} \
	return 0; \
}