#define MAX_SKINS_NUM 1024
#define MAX_SKIN_NAME_LENGTH 64
#define MAX_SKIN_SOUND_NUM 256

enum struct BodysData {
	char name[32];
	int index;
}

enum struct Skin
{
	int id;
	char name[MAX_SKIN_NAME_LENGTH];
	char uid[MAX_SKIN_NAME_LENGTH];
	char skinPath[PLATFORM_MAX_PATH];
	char armPath[PLATFORM_MAX_PATH];
	// char deathSound[PLATFORM_MAX_PATH];
	ArrayList bodys;
	StringMap sounds;
	int initBody;
	char soundOverride[MAX_SKIN_NAME_LENGTH];
	ArrayList skins;
	ArrayList groupIDs;

	bool Init(JSONObject data)
	{
		this.bodys = new ArrayList(sizeof(BodysData));
		this.sounds = new StringMap();
		this.skins = new ArrayList(sizeof(BodysData));
		this.groupIDs = new ArrayList(20);

		this.id = data.GetInt("id");
		data.GetString("name", this.name, MAX_SKIN_NAME_LENGTH);

		// attr
		JSONObject attr = view_as<JSONObject>(data.Get("attributes"));
		attr.GetString("uid", this.uid, MAX_SKIN_NAME_LENGTH);
		attr.GetString("skin_path", this.skinPath, PLATFORM_MAX_PATH);
		if (attr.HasKey("arm_path"))
			attr.GetString("arm_path", this.armPath, PLATFORM_MAX_PATH);
		// if (attr.HasKey("skin_death_sound"))
		//	 attr.GetString("skin_death_sound", this.deathSound, PLATFORM_MAX_PATH);
		if (attr.HasKey("sound_override"))
			attr.GetString("sound_override", this.soundOverride, MAX_SKIN_NAME_LENGTH);

		// groups
		if (attr.HasKey("group_ids"))
		{
			JSONArray groups = view_as<JSONArray>(attr.Get("group_ids"));
			int length = groups.Length;
			char gid[20];
			for (int i = 0; i < length; i++)
			{
				groups.GetString(i, gid, sizeof(gid));
				this.groupIDs.PushString(gid);
			}
			delete groups;
		}

		if (attr.HasKey("bodys"))
		{
			static BodysData tmp_body;
			JSONObject bodys = view_as<JSONObject>(attr.Get("bodys"));
			JSONObjectKeys bodysKeys = bodys.Keys();
			while (bodysKeys.ReadKey(tmp_body.name, sizeof(BodysData::name)))
			{
				tmp_body.index = bodys.GetInt(tmp_body.name);
				this.bodys.PushArray(tmp_body);
			}
			delete bodysKeys;
			delete bodys;

			if (attr.HasKey("init_body")) this.initBody = attr.GetInt("init_body");
		}

		if (attr.HasKey("skins"))
		{
			static BodysData tmp_skin;
			JSONObject skins = view_as<JSONObject>(attr.Get("skins"));
			JSONObjectKeys skinsKeys = skins.Keys();
			while (skinsKeys.ReadKey(tmp_skin.name, sizeof(BodysData::name)))
			{
				tmp_skin.index = skins.GetInt(tmp_skin.name);
				this.skins.PushArray(tmp_skin);
			}
			delete skinsKeys;
			delete skins;
		}

		delete attr;

		// Precache model
		if (this.skinPath[0] != '\0')
		{
			if (FileExists(this.skinPath, true))
			{
#if !defined GAME_INSURGENCY
				PrecacheModel(this.skinPath, IsSDK2013Mod() ? true : false);
#endif
			}
			else
			{
				NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", this.skinPath);
				return false;
			}
		}

#if !defined GAME_INSURGENCY
		if (this.armPath[0] != '\0')
		{
			if (FileExists(this.armPath, true))
			{
				PrecacheModel(this.armPath, IsSDK2013Mod() ? true : false);
			}
			else
			{
				NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", this.armPath);
				return false;
			}
		}
#endif

		// if (this.deathSound[0] != '\0')
		// {
		//	 char path[PLATFORM_MAX_PATH];
		//	 Format(path, sizeof(path), "sound/%s", this.deathSound);
		//	 if (FileExists(path, true))
		//		 PrecacheSound(this.deathSound);
		//	 else
		//		 NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", path);
		// }

		// Load sound
		this.LoadSounds();

		return true;
	}

	void Clear()
	{
		this.id = 0;
		this.name[0] = '\0';
		this.skinPath[0] = '\0';
		this.armPath[0] = '\0';
		// this.deathSound[0] = '\0';
		delete this.bodys;
		delete this.sounds;
		this.soundOverride[0] = '\0';
		delete this.skins;
		delete this.groupIDs;
	}

	void LoadSounds()
	{
		static char path[PLATFORM_MAX_PATH];
		Format(path, sizeof(path), "sound/newpage/models/%s", !this.soundOverride[0] ? this.uid : this.soundOverride);
		DirectoryListing dir = OpenDirectory(path, true);
		if (dir)
		{
			FileType type;
			while (dir.GetNext(path, sizeof(path), type))
			{
				static int indexs[MAX_SKIN_SOUND_NUM], size;
				char key[16], num[4], ext[6];

				// Not found
				if (path[0] == '.' || type == FileType_Unknown) continue;

				// Get sound info
				Regex regex = new Regex("([A-Za-z_]+)([0-9]*).(mp3|ogg|wav)");
				if (regex.Match(path) <= 0) continue;
				regex.GetSubString(1, key, sizeof(key));
				regex.GetSubString(2, num, sizeof(num));
				regex.GetSubString(3, ext, sizeof(ext));
				delete regex;

				static char buffer[32];
				Format(buffer, sizeof(buffer), "%s_files", key);
				if (!this.sounds.GetArray(buffer, indexs, sizeof(indexs), size))
					size = 0;
				if (size >= sizeof(indexs)) {
					NCS_LogError("Skin", "Skin.LoadSounds", "%s has too many sound files %d/%d", this.uid, size, sizeof(indexs));
					continue;
				}
				indexs[size] = StringToInt(num);
				this.sounds.SetArray(buffer, indexs, size+1);
				Format(buffer, sizeof(buffer), "%s_ext", key);
				this.sounds.SetString(buffer, ext);
			}
		}
		delete dir;
	}

	// Get sound randomly 
	bool GetSound(const char[] key, char sample[PLATFORM_MAX_PATH])
	{
		static char buffer[PLATFORM_MAX_PATH];
		static int indexs[MAX_SKIN_SOUND_NUM], length;
		Format(buffer, PLATFORM_MAX_PATH, "%s_files", key);
		if (!this.sounds.GetArray(buffer, indexs, sizeof(indexs), length))
			return false;
		if (!length) return false;

		// Get extension
		Format(buffer, PLATFORM_MAX_PATH, "%s_ext", key);
		if (!this.sounds.GetString(buffer, buffer, PLATFORM_MAX_PATH))
			return false;
		Format(buffer, PLATFORM_MAX_PATH, ".%s", buffer);

		// Get random files
		int index = GetURandomInt() % length;
		if (indexs[index] > 0) Format(buffer, PLATFORM_MAX_PATH, "%d%s", indexs[index], buffer);
		Format(sample, PLATFORM_MAX_PATH, "newpage/models/%s/%s%s", !this.soundOverride[0] ? this.uid : this.soundOverride, key, buffer);
		return true;
	}
}

methodmap SkinIndex < StringMap
{
	public SkinIndex()
	{
		return view_as<SkinIndex>(new StringMap());
	}

	public bool Add(JSONObject data)
	{
		char key[8];
		Skin s;
		if (!s.Init(data))
			return false;
		IntToString(s.id, key, sizeof(key));
		return this.SetArray(key, s, sizeof(s), true);
	}

	public bool Get(int skinID, Skin ref)
	{
		char key[8];
		IntToString(skinID, key, sizeof(key));
		return this.GetArray(key, ref, sizeof(ref));
	}

	public void Clear()
	{
		static char buffer[8]
		static Skin skin;
		StringMapSnapshot snapshot = this.Snapshot();
		for (int i = 0; i < snapshot.Length; i++)
		{
			snapshot.GetKey(i, buffer, sizeof(buffer));
			this.GetArray(buffer, skin, sizeof(skin));
			skin.Clear();
			this.Remove(buffer);
		}
		delete snapshot;
	}
}

SkinIndex g_Skins;
int g_UsersUsedSkin[MAXPLAYERS+1];

// Library
#if !defined GAME_INSURGENCY
bool isZombieReloadedLoaded = false;

void InitLibrary()
{
	isZombieReloadedLoaded = LibraryExists("zombiereloaded");
}
#endif

#include "skin/admin"
#include "skin/group"
#include "skin/block"
//#include "skin/arms"
//#include "skin/radio"
#include "skin/volume"

#if defined GAME_INSURGENCY
#include "skin/cmd.ins"
#include "skin/sound.ins"
#endif
#include "skin/sound"

#if defined GAME_INSURGENCY
#include "skin/player.ins"
#else
#include "skin/player"
#endif

#include "skin/api"
#include "skin/preview"
#include "skin/menu"
#include "skin/cmd"
#include "skin/native"
#include "skin/cookie"